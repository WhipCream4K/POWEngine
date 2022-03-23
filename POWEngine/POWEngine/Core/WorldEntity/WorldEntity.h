#pragma once

#include "POWEngine/Core/Input/InputSettings.h"
#include "POWEngine/Core/ECS/ECSTypes.h"
#include "POWEngine/Core/Components/BaseComponent.h"
#include "POWEngine/Core/Thread/SimpleThreadPool.h"
#include "POWEngine/LockFree/LFStack.h"
#include "PipelineLayer.h"
#include "POWEngine/Core/ECS/Archetype.h"

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

		// TODO: Maybe do a thread safe removing system
		void RemoveSystem(const SharedPtr<SystemBase>& system) const;

		GameObjectId GetEntityId();

		template<typename ComponentType>
		EnableIsBasedOf<BaseComponent, ComponentType, ComponentType*> AddComponentToGameObject(
			GameObjectId id,
			ComponentType&& component,
			ComponentFlag flag = ComponentFlag::Default);

		void RemoveComponentFromGameObject(GameObjectId id, ComponentTypeID componentID);


		// -------------------------------
		// -------- Pipeline -------------
		// -------------------------------
		void UpdatePipeline(PipelineLayer layer, float deltaTime);

	private:

		static bool IsDigitExistInNumber(const std::vector<ComponentTypeID>& compIds, const std::unordered_set<ComponentTypeID>& digit);
		SharedPtr<Archetype> CreateArchetypeWithTypes(const std::vector<ComponentTypeID>& typeID);
		static std::string CreateStringFromNumVector(const std::vector<ComponentTypeID>& numList);

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
		std::unordered_map<ComponentTypeID, SharedPtr<BaseComponent>> m_ComponentMap;
		// Has loop iteration, need to take care of run-time write
		//std::vector<SharedPtr<Archetype>> m_ArchetypesPool;
		std::unordered_map<std::string, SharedPtr<Archetype>> m_ArchetypesPool;
		std::unordered_map<std::string, SharedPtr<Archetype>> m_PendingArchetypesMap;

		// -------------------------------
		// --------- Entity --------------
		// -------------------------------

		// No loop iteration
		// Map that saves which GameObject belongs to which archetype at what index
		std::unordered_map<GameObjectId, GameObjectInArchetypeRecord> m_GameObjectRecords;
		// Although this is not thread safe but the initialization of GameObject should be in main thread
		GameObjectId m_GameObjectCounter{};
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
		const size_t typeId{ Component<ComponentType>::GetId() };
		m_ComponentMap.try_emplace(typeId, std::make_shared<Component<ComponentType>>());
	}

	template <typename ComponentType>
	EnableIsBasedOf<BaseComponent, ComponentType, ComponentType*> WorldEntity::AddComponentToGameObject(GameObjectId id,
		ComponentType&& component, ComponentFlag flag)
	{
		const ComponentTypeID componentId{ BaseComponent::GetId<ComponentType>() | flag };

		const auto& gameObjectRecord{ m_GameObjectRecords.find(id) };

		// This GameObject doesn't exist
		if (gameObjectRecord == m_GameObjectRecords.end())
			return nullptr;

		// 1. Check if this GameObject already exist in any archetype
		if (const SharedPtr<Archetype> oldArchetype{ gameObjectRecord->second.Archetype.lock() })
		{
			// 1.1 if so check if this component is already in register in this archetype
			if (std::ranges::find(oldArchetype->Types, componentId) != oldArchetype->Types.end()) // since c++20
			{
				// return null because we don't support multiple instances of the same component
				return nullptr;
			}

			// 1.2 if the GameObject is already existed in another archetype
			// remove the GameObject from the old archetype, change the old archetype id and data alignment
			// then create a new archetype for this GameObject

			const SharedPtr<Archetype> newArchetype{ CreateArchetypeWithTypes(oldArchetype->Types) };
			
			
		}
		else
		{
			// 1.2 Create a new archetype for this GameObject
			const std::vector<ComponentTypeID> compTypes{{componentId}}; // initializer list
			//const std::string archetypeKey{ CreateStringFromNumVector(compTypes) };

			if(const SharedPtr<Archetype> newArchetype{CreateArchetypeWithTypes(compTypes)})
			{
				newArchetype->GameObjectIds.emplace_back(id);
			}
		}
	}
}


