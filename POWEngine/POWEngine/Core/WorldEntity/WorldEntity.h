#pragma once

#include "POWEngine/Core/Input/InputSettings.h"
#include "POWEngine/Core/ECS/ECSTypes.h"
#include "POWEngine/Core/Components/BaseComponent.h"
#include "POWEngine/Core/Thread/SimpleThreadPool.h"
#include "POWEngine/LockFree/LFStack.h"
#include "PipelineLayer.h"
#include "POWEngine/Core/ECS/Archetype.h"
#include "POWEngine/Core/Components/ChildComponentTraits.h"

namespace powe
{
	class SystemBase;
	class WorldEntity final
	{
		using SystemPipelines = std::array<std::vector<SharedPtr<SystemBase>>, size_t(PipelineLayer::Count)>;

	public:

		// TODO: Fix rule of 5
		WorldEntity();
		WorldEntity(const WorldEntity&) = delete;
		WorldEntity& operator=(const WorldEntity&) = delete;
		WorldEntity(WorldEntity&&) noexcept = default;
		WorldEntity& operator=(WorldEntity&&) noexcept = default;
		~WorldEntity() = default;

	public:

		InputSettings& GetInputSettings() { return  m_InputSettings; }

		// Lock-free add system
		template<typename SystemType, typename ...Args>
		EnableIsBasedOf<SystemBase, SystemType, WeakPtr<SystemBase>> AddSystem(Args&&... args);

		// TODO: Maybe do a thread safe registering component
		template<typename ComponentType>
		EnableIsBasedOf<BaseComponent, ComponentType> RegisterComponent();

		void RegisterGameObject(GameObjectID id);

		// TODO: Maybe do a thread safe removing system
		void RemoveSystem(const SharedPtr<SystemBase>& system) const;

		const auto& GetComponentMap() const { return m_ComponentTraitsMap; }

		GameObjectID GetNewEntityID();

		template<typename ComponentType>
		EnableIsBasedOf<BaseComponent, ComponentType, ComponentType*> AddComponentToGameObject(
			GameObjectID id,
			ComponentType&& component,
			ComponentFlag flag = ComponentFlag::Default);

		template<typename ComponentType>
		EnableIsBasedOf<BaseComponent, ComponentType, ComponentType*> AddComponentSparseToGameObject(
			GameObjectID id,
			ComponentType&& component,
			ComponentFlag flag = ComponentFlag::Default);

		template<typename ComponentType>
		EnableIsBasedOf<BaseComponent, ComponentType, ComponentType*> GetComponentByGameObject(GameObjectID id);

		void RemoveComponentByID(GameObjectID id, ComponentTypeID componentID);

		template<typename Component>
		void RemoveComponentByType(GameObjectID id);

		void RemoveGameObject(GameObjectID id, bool removeRecord = true);

		// -------------------------------
		// -------- Pipeline -------------
		// -------------------------------
		void UpdatePipeline(PipelineLayer layer, float deltaTime);

		SharedPtr<BaseComponent> GetBaseComponentByID(ComponentTypeID id) const;
		SharedPtr<Archetype> GetArchetypeByGameObject(GameObjectID id) const;
		SizeType GetComponentSize(ComponentTypeID id) const;

	private:

		static bool IsDigitExistInNumber(const std::vector<ComponentTypeID>& compIds, const std::unordered_set<ComponentTypeID>& digit);
		SharedPtr<Archetype> CreateArchetypeWithTypes(const std::vector<ComponentTypeID>& typeID);
		//SharedPtr<Archetype> CreateArchetypeWithKey(const std::string& key);
		static std::string CreateStringFromNumVector(const std::vector<ComponentTypeID>& numList);
		SharedPtr<Archetype> UpdatePendingArchetypeKey(const std::string& targetKey, const std::string& newKey);
		SharedPtr<Archetype> GetArchetypeFromPendingList(const std::string& key);
		SharedPtr<Archetype> GetArchetypeFromActiveList(const std::string& key) const;

		void RemoveArchetype(const std::string& key);

		template<typename ComponentType>
		ComponentType* AllocateComponentData(
			const SharedPtr<Archetype>& targetArchetype,
			const SharedPtr<Archetype>& oldArchetype,
			int indexInArchetype,
			ComponentType&& component);

	private:

		// This class will get execute almost every frame,
		// it makes sense that it would live on the stack with WorldEntity
		InputSettings m_InputSettings;

		OwnedPtr<SimpleThreadPool> m_SimpleThreadPool;

		// ========== ECS ================

		// -------------------------------
		// --------- System --------------
		// -------------------------------

		// Has loop iteration, need to take care of run-time write
		SystemPipelines m_SystemPipeline;
		LFStack<SharedPtr<SystemBase>> m_PendingAddSystem;

		// -------------------------------
		// --------- Component -----------
		// -------------------------------

		// No loop iteration
		// Save data when component first created
		std::unordered_map<ComponentTypeID, SharedPtr<BaseComponent>> m_ComponentTraitsMap;
		// Has loop iteration, need to take care of run-time write
		//std::vector<SharedPtr<Archetype>> m_ArchetypesPool;
		std::unordered_map<std::string, SharedPtr<Archetype>> m_ArchetypesPool;
		std::unordered_map<std::string, SharedPtr<Archetype>> m_PendingAddArchetypes;
		std::unordered_set<std::string> m_PendingRemoveArchetypes;
		std::unordered_map<ComponentTypeID, SizeType> m_SparseComponentEmptyPointer;
		std::unordered_map<ComponentTypeID, SharedPtr<RawByte[]>> m_SparseComponent;
		//std::vector<ChildComponentTraits> m_ChildComponents;

		// -------------------------------
		// --------- Entity --------------
		// -------------------------------

		// No loop iteration
		// Map that saves which GameObject belongs to which archetype at what index
		std::unordered_map<GameObjectID, GameObjectInArchetypeRecord> m_GameObjectRecords;
		// Although this is not thread safe but the initialization of GameObject should be in main thread
		GameObjectID m_GameObjectCounter{};
	};

	template <typename SystemType, typename ... Args>
	EnableIsBasedOf<SystemBase, SystemType, WeakPtr<SystemBase>> WorldEntity::AddSystem(Args&&... args)
	{
		SharedPtr<SystemBase> system{ std::make_shared<SystemType>(std::forward<Args>(args)...) };
		m_PendingAddSystem.Push(system);
		return system;
	}

	template <typename ComponentType>
	EnableIsBasedOf<BaseComponent, ComponentType> WorldEntity::RegisterComponent()
	{
		//const size_t typeId{ BaseComponent::GetI GetId() };
		const ComponentTypeID componentID{ BaseComponent::GetId<ComponentType>() };
		m_ComponentTraitsMap.try_emplace(componentID, std::make_shared<ComponentType>());
	}

	template <typename ComponentType>
	EnableIsBasedOf<BaseComponent, ComponentType, ComponentType*> WorldEntity::AddComponentToGameObject(GameObjectID id,
		ComponentType&& component, ComponentFlag)
	{
		const ComponentTypeID componentId{ BaseComponent::GetId<ComponentType>() };

		// 0. Register component automatically
		if (!m_ComponentTraitsMap.contains(componentId))
			RegisterComponent<ComponentType>();

		const auto& gameObjectRecord{ m_GameObjectRecords.find(id) };

		// This GameObject doesn't exist
		if (gameObjectRecord == m_GameObjectRecords.end())
			return nullptr;

		SharedPtr<Archetype> newArchetype{};
		SharedPtr<Archetype> oldArchetype{};
		int indexInOldArchetype{};
		bool isReUsing{};

		// 1. Check if this GameObject already exist in any archetype
		if (const SharedPtr<Archetype> recordedArchetype{ gameObjectRecord->second.Archetype.lock() })
		{
			// 1.1 if so check if this component is already in register in this archetype
			if (std::ranges::find(recordedArchetype->Types, componentId) != recordedArchetype->Types.end()) // since c++20
			{
				// return null because we don't support multiple instances of the same component
				return nullptr;
			}

			// 1.2 if the GameObject is already existed in another archetype
			// remove the GameObject from the old archetype, change the old archetype id and data alignment
			// then create a new archetype for this GameObject

			std::vector<ComponentTypeID> newCompTypes{ recordedArchetype->Types };
			newCompTypes.emplace_back(componentId);
			const std::string newArchetypeKey{ CreateStringFromNumVector(newCompTypes) };
			const std::string oldArchetypeKey{ CreateStringFromNumVector(recordedArchetype->Types) };

			// check if this archetype is in the active list or not
			auto activeArchetype{ GetArchetypeFromActiveList(oldArchetypeKey) };
			if (activeArchetype == recordedArchetype)
			{
				// this one is active archetype we need to create a new archetype,
				activeArchetype = GetArchetypeFromActiveList(newArchetypeKey);
				newArchetype = CreateArchetypeWithTypes(newCompTypes);

				if (activeArchetype)
				{
					newArchetype->GameObjectIds = recordedArchetype->GameObjectIds;
					newArchetype->ComponentData = newArchetype->CopyComponentData(*recordedArchetype, *this);
				}

				RemoveGameObject(id,false);
			}
			else // this archetype is still in pending we can add it directly
			{
				// if it's not alone then we create or get a new archetype and remove the gameobject from old archetype
				if(recordedArchetype->GameObjectIds.size() != 1)
				{
					newArchetype = CreateArchetypeWithTypes(newCompTypes);
					RemoveGameObject(id, false);
				}
				else
				{
					newArchetype = recordedArchetype;
					newArchetype->Types = newCompTypes; // added new types
					UpdatePendingArchetypeKey(oldArchetypeKey, newArchetypeKey);
					isReUsing = true;
				}
			}

			oldArchetype = recordedArchetype;

			indexInOldArchetype = gameObjectRecord->second.IndexInArchetype;

		}
		else
		{
			// 1.2 Create a new archetype for this GameObject
			const std::vector<ComponentTypeID> compTypes{ {componentId} }; // initializer list
			const std::string archetypeKey{ CreateStringFromNumVector(compTypes) };

			const auto activeArchetype{ GetArchetypeFromActiveList(archetypeKey) };

			newArchetype = CreateArchetypeWithTypes(compTypes);
			if (activeArchetype)
			{
				newArchetype->GameObjectIds = activeArchetype->GameObjectIds;
				newArchetype->ComponentData = newArchetype->CopyComponentData(*activeArchetype, *this);
				oldArchetype = activeArchetype;
			}
		}

		ComponentType* outPointer{};
		// 2. Re-allocate the data and align according to our new Archetype
		outPointer = AllocateComponentData(newArchetype, oldArchetype,indexInOldArchetype, std::forward<ComponentType>(component));

		if (!isReUsing)
		{
			//4. Save this GameObject to GameObjectRecord
			gameObjectRecord->second.Archetype = newArchetype;
			gameObjectRecord->second.IndexInArchetype = int(newArchetype->GameObjectIds.size());

			//5. Add this GameObject ID to our new archetype
			newArchetype->GameObjectIds.emplace_back(id);

		}


		return outPointer;

	}

	template <typename ComponentType>
	EnableIsBasedOf<BaseComponent, ComponentType, ComponentType*> WorldEntity::GetComponentByGameObject(GameObjectID id)
	{
		const auto gameObjectItr{ m_GameObjectRecords.find(id) };
		const ComponentTypeID compID{ BaseComponent::GetId<ComponentType>() };
		
		if(gameObjectItr != m_GameObjectRecords.end())
		{
			if(const auto archetype{ gameObjectItr->second.Archetype.lock() })
			{
				SizeType accumulateOffset{};
				RawByte* startAddress{ &archetype->ComponentData[gameObjectItr->second.IndexInArchetype * archetype->SizeOfComponentsBlock] };
				for (const auto& type : archetype->Types)
				{
					if (type == compID)
						return reinterpret_cast<ComponentType*>(startAddress + accumulateOffset);
					accumulateOffset += GetComponentSize(type);
				}	
			}
		}

		return nullptr;
	}

	template <typename Component>
	void WorldEntity::RemoveComponentByType(GameObjectID id)
	{
		const ComponentTypeID componentID{ BaseComponent::GetId<Component>() };
		RemoveComponentByID(id, componentID);
	}

	template <typename ComponentType>
	ComponentType* WorldEntity::AllocateComponentData(
		const SharedPtr<Archetype>& targetArchetype,
		const SharedPtr<Archetype>& oldArchetype,
		int indexInArchetype,
		ComponentType&& component)
	{
		const size_t newComponentSize{ sizeof(ComponentType) };

		//SharedPtr<RawByte[]> targetComponentData{ targetArchetype->ComponentData };
		const SizeType currentOccupiedSize{ targetArchetype->SizeOfComponentsBlock * SizeType(targetArchetype->GameObjectIds.size()) };

		if (currentOccupiedSize + newComponentSize >= targetArchetype->TotalAllocatedData || !targetArchetype->ComponentData)
		{
			const SizeType newSize{ targetArchetype->SizeOfComponentsBlock * SizeType(targetArchetype->GameObjectIds.size() + 2) };
			targetArchetype->AllocateComponentData(newSize, *this);
		}

		RawByte* newDataAddress{ &targetArchetype->ComponentData[targetArchetype->GameObjectIds.size() * targetArchetype->SizeOfComponentsBlock] };
		SizeType accumulateOffset{};

		if (oldArchetype && oldArchetype->ComponentData)
		{

			RawByte* startAddress{ &oldArchetype->ComponentData[indexInArchetype * oldArchetype->SizeOfComponentsBlock] };
			RawByte* endAddress{ newDataAddress };

			for (const auto& componentID : oldArchetype->Types)
			{
				const SharedPtr<BaseComponent> thisComponent{ m_ComponentTraitsMap.at(componentID) };
				thisComponent->MoveData(startAddress + accumulateOffset, endAddress + accumulateOffset);
				accumulateOffset += thisComponent->GetSize();
			}

		}

		ComponentType* outComponentPointer{};
		outComponentPointer = new (newDataAddress + accumulateOffset) ComponentType(component);

		return outComponentPointer;
	}

	//template <typename ComponentType>
	//ComponentType* WorldEntity::AllocateComponentData(const SharedPtr<Archetype>& targetArchetype,
	//	int indexInOldArchetype, ComponentType&& component)
	//{
	//	const size_t newComponentSize{ sizeof(ComponentType) };
	//	ComponentType* outPointer{};

	//	SharedPtr<RawByte[]> targetComponentData{ targetArchetype->ComponentData };
	//	const SizeType oldComponentsDataTotalSize{ targetArchetype->SizeOfComponentsBlock * SizeType(targetArchetype->GameObjectIds.size()) };

	//	if(oldComponentsDataTotalSize + newComponentSize > targetArchetype->TotalAllocatedData || !targetArchetype->ComponentData)
	//	{
	//		const SizeType newSize{ oldComponentsDataTotalSize + (targetArchetype->SizeOfComponentsBlock) * 3 };

	//		targetComponentData = SharedPtr<RawByte[]>{ new RawByte[newSize]{} };

	//		//if(targetArchetype->ComponentData)
	//		//{
	//		//	std::copy_n(targetArchetype->ComponentData.get(),
	//		//		oldComponentsDataTotalSize,
	//		//		targetComponentData.get());
	//		//}

	//		targetArchetype->TotalAllocatedData = newSize;
	//		//targetArchetype->ComponentData = targetComponentData;
	//	}

	//	if(targetArchetype->ComponentData)
	//	{
	//		for (const auto& componentID : targetArchetype->Types)
	//		{
	//			
	//		}			
	//	}
	//}
}


