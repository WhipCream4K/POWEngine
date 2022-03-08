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
	using SystemPipeline = std::array<std::vector<SharedPtr<SystemBase>>, size_t(PipelineLayer::Count)>;

	class WorldEntity final
	{
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
		template<typename SystemType,typename ...Args>
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

		// -------------------------------
		// -------- Pipeline -------------
		// -------------------------------
		void UpdatePipeline(PipelineLayer layer, float deltaTime);

	private:

		static bool IsDigitExistInNumber(const std::vector<ComponentTypeId>& compIds, const std::unordered_set<ComponentTypeId>& digit);

	private:

		// This class will get execute almost every frame,
		// it makes sense that it would live on the stack with WorldEntity
		InputSettings m_InputSettings;
		
		SharedPtr<SimpleThreadPool> m_SimpleThreadPool;

		// ========== ECS ================

		// -------------------------------
		// --------- System --------------
		// -------------------------------

		// Has loop iteration, need to take care of run-time write
		SystemPipeline m_SystemPipeline;
		LFStack<SharedPtr<SystemBase>> m_PendingAddSystem;

		// -------------------------------
		// --------- Component -----------
		// -------------------------------

		// No loop iteration
		// Save data when component first created
		std::unordered_map<ComponentTypeId, SharedPtr<BaseComponent>> m_ComponentMap;
		// Has loop iteration, need to take care of run-time write
		// Buffer of archetypes by their combined component type id
		std::vector<SharedPtr<Archetype>> m_ArchetypesPool;

		std::unordered_map<ComponentTypeId,size_t> m_PendingArchetypesMap;

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
		const ComponentTypeId componentId{ BaseComponent::GetId<ComponentType>() | flag };

		const auto& gameObjectRecord{ m_GameObjectRecords.find(id) };

		// This GameObject doesn't exist
		if (gameObjectRecord == m_GameObjectRecords.end())
			return nullptr;

		// 1. Check if this GameObject already exist in any archetype

		if(const SharedPtr<Archetype> oldArchetype{ gameObjectRecord->second.Archetype.lock() })
		{
			// 1.1 if so check if this component is already in register in this archetype
			if (std::ranges::find(oldArchetype->Types, componentId) != oldArchetype->Types.end()) // since c++20
			{
				// return null because we don't support multiple instances of the same component
				return nullptr;
			}

			// 1.2 Create a new archetype and add the id to the archetype
			ArchetypeBuffer newArchetype{};
			
			
		}
		else
		{
			
		}
	}
}


