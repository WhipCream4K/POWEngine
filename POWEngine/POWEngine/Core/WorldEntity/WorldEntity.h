#pragma once

#include "POWEngine/Core/Input/InputSettings.h"
#include "POWEngine/ECS/Archetype.h"
#include "POWEngine/Core/Components/BaseComponent.h"
#include "POWEngine/Core/Thread/SimpleThreadPool.h"
#include "POWEngine/LockFree/LFStack.h"
#include "PipelineLayer.h"
#include "POWEngine/ECS/ECSUtils.h"
#include "POWEngine/ECS/SparseComponentManager.h"

namespace powe
{
	class SystemBase;
	class WorldEntity final
	{
		using SystemPipelines = std::array<std::vector<SharedPtr<SystemBase>>, size_t(PipelineLayer::Count)>;

	public:

		WorldEntity();
		WorldEntity(const WorldEntity&) = delete;
		WorldEntity& operator=(const WorldEntity&) = delete;
		WorldEntity(WorldEntity&&) noexcept = delete;
		WorldEntity& operator=(WorldEntity&&) noexcept = delete;
		~WorldEntity();

	public:

		InputSettings& GetInputSettings() { return  m_InputSettings; }

		// Lock-free add system
		void RegisterSystem(PipelineLayer layer, const SharedPtr<SystemBase>& system);
		void RemoveSystem(const SharedPtr<SystemBase>& system);

		// TODO: Maybe do a thread safe registering component
		template<typename ComponentType>
		EnableIsBasedOf<BaseComponent, ComponentType> RegisterComponent();

		void RegisterGameObject(GameObjectID id);

		GameObjectID GetNewEntityID();

		template<typename ComponentType>
		EnableIsBasedOf<BaseComponent, ComponentType, ComponentType*> AddComponentToGameObject(
			GameObjectID id,
			ComponentType&& component,
			ComponentFlag componentFlag = ComponentFlag::Default);

		template<typename ComponentType>
		EnableIsBasedOf<BaseComponent, ComponentType, ComponentType*> GetComponent(GameObjectID id) const;


		const SparseComponentManager& GetSparseComponentManager() const { return m_SparseComponentManager; }

		template<typename Component>
		void RemoveComponentByType(GameObjectID id);

		void RemoveGameObject(GameObjectID id, bool removeRecord = true);

		// -------------------------------
		// -------- Pipeline -------------
		// -------------------------------
		void UpdatePipeline(PipelineLayer layer, float deltaTime);
		void ResolveEntities();

		template<typename ComponentType>
		EnableIsBasedOf<BaseComponent, ComponentType, ComponentType*> FindUniqueComponent(powe::GameObjectID& ownerID) const;

		SharedPtr<Archetype> GetArchetypeByGameObject(GameObjectID id) const;
		const std::unordered_map<std::string, SharedPtr<Archetype>> GetActiveArchetypes() const { return m_ArchetypesPool; }
		SharedPtr<BaseComponent> GetComponentTrait(ComponentTypeID id) const;

		static std::string CreateStringFromNumVector(const std::vector<ComponentTypeID>& numList);
		bool GetGameObjectRecords(GameObjectID id, GameObjectRecord& outRecord) const;
		GameObjectRecord& GetRefGameObjectRecord(GameObjectID id);

#ifdef RUNTIME_TEST
	public:
		void InternalAddGameObjectToPipeline();
		void InternalRemoveGameObjectFromPipeline();
		void InternalRemoveComponentFromGameObject();
#else

	private:

		void InternalAddGameObjectToPipeline();
		void InternalAddSystemToPipeline();
		void InternalRemoveSystemFromPipeline();
		void InternalRemoveGameObjectFromPipeline();
		void InternalRemoveComponentFromGameObject();
		void ClearArchetype();
		void ClearEmptyArchetype();

#endif

	private:

		//SharedPtr<Archetype> CreateArchetypeWithTypes(const std::vector<ComponentTypeID>& typeID);
		//SharedPtr<Archetype> UpdatePendingArchetypeKey(const std::string& targetKey, const std::string& newKey);
		//SharedPtr<Archetype> GetArchetypeFromPendingList(const std::string& key);
		//void RemoveArchetype(const std::string& key);

		void RemoveComponentByID(GameObjectID id, ComponentTypeID componentID);

		SharedPtr<Archetype> GetArchetypeFromActiveList(const std::string& key) const;
		const auto& GetComponentMap() const { return m_ComponentTraitsMap; }
		bool GetPreArchetypeTrait(GameObjectID id, PreArchetypeTrait& outTrait) const;

		// Actually trying to remove gameobjects from pending delete list
		void AddGameObjectToRecordRemoveList(GameObjectID id);
		void AddGameObjectToArchetypeRemoveList(const std::string& archetypeKey, GameObjectID id);
		void AddComponentToGameObjectRemoveList(GameObjectID id, ComponentTypeID componentTypeID);

		void AddArchetype(const std::string& key, const SharedPtr<Archetype>& archetype);

		void AddPreArchetype(
			GameObjectID gameObjectID,
			ComponentTypeID componentID,
			const SharedPtr<RawByte[]>& reservedComponentData);

		void RemoveComponentFromPreArchetype(GameObjectID id, ComponentTypeID componentTypeId);
		void RemoveGameObjectFromPreArchetype(GameObjectID id);

		template<typename ComponentType>
		ComponentType* AllocateComponentData(
			const SharedPtr<Archetype>& targetArchetype,
			const SharedPtr<Archetype>& oldArchetype,
			int indexInArchetype,
			ComponentType&& component);

		void DestroyAllComponentDataInGameObject(
			const Archetype& archetype,
			int index,
			GameObjectID id,
			const std::vector<ComponentTypeID>& components,
			bool callInternDestroy = true);


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

		struct SystemTrait
		{
			SharedPtr<SystemBase> system{};
			PipelineLayer layer{};
		};

		LFStack<SystemTrait> m_PendingAddSystem;
		LFStack<SharedPtr<SystemBase>> m_PendingDeleteSystem;

		// -------------------------------
		// --------- Component -----------
		// -------------------------------
		// No loop iteration
		// Save data when component first created
		std::unordered_map<ComponentTypeID, SharedPtr<BaseComponent>> m_ComponentTraitsMap;
		std::unordered_map<GameObjectID, std::vector<ComponentTypeID>> m_PendingDeleteComponentsFromGameObject;
		SparseComponentManager m_SparseComponentManager;


		// -------------------------------
		// --------- Entity --------------
		// -------------------------------

		// No loop iteration
		// Map that saves which GameObject belongs to which archetype at what index
		std::unordered_map<GameObjectID, GameObjectRecord> m_GameObjectRecords;
		std::unordered_map<std::string, std::vector<GameObjectID>> m_PendingDeleteGameObjectsFromArchetype;
		std::vector<GameObjectID> m_PendingDeleteGameObjectsFromRecord;

		// ------------------------------
		// --------- Archetype ----------
		// ------------------------------
		// Has loop iteration, need to take care of run-time write
		std::unordered_map<std::string, SharedPtr<Archetype>> m_ArchetypesPool;
		std::unordered_map<GameObjectID, PreArchetypeTrait> m_PreArchetypes;

		// Although this is not thread safe but the initialization of GameObject should be in main thread
		GameObjectID m_GameObjectCounter{};

	};

	template <typename ComponentType>
	EnableIsBasedOf<BaseComponent, ComponentType> WorldEntity::RegisterComponent()
	{
		//const size_t typeId{ BaseComponent::GetI GetId() };
		const ComponentTypeID componentID{ BaseComponent::GetId<ComponentType>() };
		m_ComponentTraitsMap.try_emplace(componentID, std::make_shared<ComponentType>());
	}

	template <typename ComponentType>
	EnableIsBasedOf<BaseComponent, ComponentType, ComponentType*> WorldEntity::AddComponentToGameObject(GameObjectID id,
		ComponentType&& component, ComponentFlag componentFlag)
	{
		const ComponentTypeID componentId{ BaseComponent::GetId<ComponentType>() | ComponentTypeID(componentFlag) };

		// 0. Register component automatically
		if (!m_ComponentTraitsMap.contains(componentId))
			RegisterComponent<ComponentType>();

		GameObjectRecord gameObjectRecord{};
		if (!GetGameObjectRecords(id, gameObjectRecord))
			return nullptr;

		if (const auto oldArchetype{ gameObjectRecord.Archetype.lock() })
		{
			const std::string oldArchetypeKey{ CreateStringFromNumVector(oldArchetype->Types) };

			// 1.1 if so check if this component is already in register in this archetype
			if (std::ranges::find(oldArchetype->Types, componentId) != oldArchetype->Types.end()) // since c++20
			{
				// return null because we don't support multiple instances of the same component
				return nullptr;
			}

			// we need to remove this gameobject from its own archetype as well
			//AddGameObjectToArchetypeRemoveList(oldArchetypeKey, id);
		}

		PreArchetypeTrait preArchetypeTrait{};
		if (GetPreArchetypeTrait(id, preArchetypeTrait))
		{
			// if the gameobject has been prepared to be added into the pipeline then check if it already has one
			// if true then return it
			if (preArchetypeTrait.componentData.contains(componentId))
				return  reinterpret_cast<ComponentType*>(preArchetypeTrait.componentData[componentId].get());
		}

		// Create data for the pre-archetype container
		const SharedPtr<RawByte[]> componentData{ SharedPtr<RawByte[]>{new RawByte[sizeof(ComponentType)]{}} };

		// Initialize data
		ComponentType* outPointer{ new (componentData.get())
			ComponentType(std::forward<ComponentType>(component)) };

		AddPreArchetype(id, componentId, componentData);

		return outPointer;

#pragma region OLDCODE

		// 1. Check if this GameObject already exist in any archetype
		//if (const SharedPtr<Archetype> recordedArchetype{ gameObjectRecord.Archetype.lock() })
		//{
		//	// 1.1 if so check if this component is already in register in this archetype
		//	if (std::ranges::find(recordedArchetype->Types, componentId) != recordedArchetype->Types.end()) // since c++20
		//	{
		//		// return null because we don't support multiple instances of the same component
		//		return nullptr;
		//	}

		//	// 1.2 if the GameObject is already existed in another archetype
		//	// remove the GameObject from the old archetype, change the old archetype id and data alignment
		//	// then create a new archetype for this GameObject

		//	std::vector<ComponentTypeID> newCompTypes{ recordedArchetype->Types };
		//	newCompTypes.emplace_back(componentId);
		//	const std::string newArchetypeKey{ CreateStringFromNumVector(newCompTypes) };
		//	const std::string oldArchetypeKey{ CreateStringFromNumVector(recordedArchetype->Types) };

		//	// check if this archetype is in the active list or not
		//	auto activeArchetype{ GetArchetypeFromActiveList(oldArchetypeKey) };
		//	if (activeArchetype == recordedArchetype)
		//	{
		//		// this one is active archetype we need to create a new archetype,
		//		activeArchetype = GetArchetypeFromActiveList(newArchetypeKey);
		//		newArchetype = CreateArchetypeWithTypes(newCompTypes);

		//		if (activeArchetype)
		//		{
		//			newArchetype->GameObjectIds = recordedArchetype->GameObjectIds;
		//			newArchetype->ComponentData = newArchetype->CopyComponentData(*recordedArchetype, *this);
		//		}

		//		//RemoveGameObject(id,false);
		//		shouldRemoveGameObject = true;
		//	}
		//	else // this archetype is still in pending we can add it directly
		//	{
		//		// if it's not alone then we create or get a new archetype and remove the gameobject from old archetype
		//		if(recordedArchetype->GameObjectIds.size() != 1)
		//		{
		//			newArchetype = CreateArchetypeWithTypes(newCompTypes);
		//			//RemoveGameObject(id, false);
		//			shouldRemoveGameObject = true;
		//		}
		//		else
		//		{
		//			newArchetype = recordedArchetype;
		//			newArchetype->Types = newCompTypes; // added new types
		//			UpdatePendingArchetypeKey(oldArchetypeKey, newArchetypeKey);
		//			isReUsing = true;
		//		}
		//	}

		//	oldArchetype = recordedArchetype;

		//	indexInOldArchetype = gameObjectRecord.IndexInArchetype;

		//}
		//else
		//{
		//	// 1.2 Create a new archetype for this GameObject
		//	const std::vector<ComponentTypeID> compTypes{ {componentId} }; // initializer list
		//	const std::string archetypeKey{ CreateStringFromNumVector(compTypes) };

		//	const auto activeArchetype{ GetArchetypeFromActiveList(archetypeKey) };

		//	newArchetype = CreateArchetypeWithTypes(compTypes);
		//	if (activeArchetype)
		//	{
		//		newArchetype->GameObjectIds = activeArchetype->GameObjectIds;
		//		newArchetype->ComponentData = newArchetype->CopyComponentData(*activeArchetype, *this);
		//		oldArchetype = activeArchetype;
		//	}
		//}

		//ComponentType* outPointer{};
		//// 2. Re-allocate the data and align according to our new Archetype
		//outPointer = AllocateComponentData(newArchetype, oldArchetype,indexInOldArchetype, std::forward<ComponentType>(component));

		//if(shouldRemoveGameObject)
		//	RemoveGameObject(id, false);

		//if (!isReUsing)
		//{
		//	//4. Save this GameObject to GameObjectRecord
		//	gameObjectRecord.Archetype = newArchetype;
		//	gameObjectRecord-.IndexInArchetype = int(newArchetype->GameObjectIds.size());

		//	//5. Add this GameObject ID to our new archetype
		//	newArchetype->GameObjectIds.emplace_back(id);

		//}

		//return outPointer;

#pragma endregion

	}

	template <typename ComponentType>
	EnableIsBasedOf<BaseComponent, ComponentType, ComponentType*> WorldEntity::GetComponent(GameObjectID id) const
	{
		const ComponentTypeID compID{ BaseComponent::GetId<ComponentType>() };

		GameObjectRecord gbRecords{};
		if (GetGameObjectRecords(id, gbRecords))
		{
			if (const SharedPtr<Archetype> archetype{ gbRecords.Archetype.lock() })
			{
				const auto findItr{ archetype->ComponentOffsets.find(compID) };
				if (findItr != archetype->ComponentOffsets.end())
				{
					//RawByte* sourceAddress{ &archetype->ComponentData[
					//	gbRecords.IndexInArchetype * archetype->SizeOfComponentsBlock
					// + findItr->second] };

					// check the key if it's a sparse component or not
					if (!IsThisComponentSparse(findItr->first))
					{
						return reinterpret_cast<ComponentType*>(&archetype->ComponentData[
							gbRecords.IndexInArchetype * archetype->SizeOfComponentsBlock
								+ findItr->second // offsets
						]);
					}

					// if it's a Sparse component
					// get the handle from the sourceAddress then use that to get a real component address
					//const SparseHandle* handle{ reinterpret_cast<SparseHandle*>(sourceAddress) };
					RawByte* realCompData{ m_SparseComponentManager.GetComponentData<ComponentType>(id,compID) };

					return reinterpret_cast<ComponentType*>(realCompData);
				}


			}

			// if some how user ask for a component before the gameobject got registered to the pipeline

			PreArchetypeTrait preArchetypeTrait{};
			if (GetPreArchetypeTrait(id, preArchetypeTrait))
			{
				const auto findItr{ preArchetypeTrait.componentData.find(compID) };
				if (findItr != preArchetypeTrait.componentData.end())
					return reinterpret_cast<ComponentType*>(findItr->second.get());
			}

		}

		return nullptr;
	}

	template <typename Component>
	void WorldEntity::RemoveComponentByType(GameObjectID id)
	{
		const ComponentTypeID componentID{ BaseComponent::GetId<Component>() };

		Component* data{ GetComponent<Component>(id) };
		if(data)
		{
			const SharedPtr<BaseComponent> thisComponent{ GetComponentTrait(componentID) };
			thisComponent->InternalDestroy(reinterpret_cast<RawByte*>(data),*this, id);

			RemoveComponentByID(id, componentID);
		}

	}

	template <typename ComponentType>
	EnableIsBasedOf<BaseComponent, ComponentType, ComponentType*> WorldEntity::FindUniqueComponent(powe::GameObjectID& ownerID) const
	{
		const ComponentTypeID componentTypeId{ BaseComponent::GetId<ComponentType>() };

		for (const auto& [id, gbRecords] : m_GameObjectRecords)
		{
			ownerID = id;
			if (const auto archetype = gbRecords.Archetype.lock())
			{
				RawByte* outData{};
				const auto findItr{ archetype->ComponentOffsets.find(componentTypeId) };
				if (findItr == archetype->ComponentOffsets.end())
					continue;

				if (IsThisComponentSparse(findItr->second))
					outData = m_SparseComponentManager.GetComponentData<ComponentType>(id, componentTypeId);
				else
					outData = &archetype->ComponentData[gbRecords.IndexInArchetype * archetype->SizeOfComponentsBlock + findItr->second];

				return reinterpret_cast<ComponentType*>(outData);

			}

			// if there's no archetype then go for pre archetype
			PreArchetypeTrait preArchetypeTrait{};
			if (GetPreArchetypeTrait(id, preArchetypeTrait))
			{
				const auto findItr{ preArchetypeTrait.componentData.find(componentTypeId) };
				if (findItr != preArchetypeTrait.componentData.end())
					return reinterpret_cast<ComponentType*>(findItr->second.get());
			}
		}

		return nullptr;
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


