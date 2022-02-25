#pragma once

#include "POWEngine/Core/Input/InputSettings.h"
#include "POWEngine/Core/ECS/ECSTypes.h"
#include "POWEngine/Core/Components/BaseComponent.h"
#include "POWEngine/Core/Thread/SimpleThreadPool.h"
#include "POWEngine/LockFree/LFStack.h"
#include "PipelineLayer.h"

namespace powe
{
	class SystemBase;
	using SystemPipeline = std::array<std::vector<SharedPtr<SystemBase>>, size_t(PipelineLayer::Count)>;

	class WorldEntity final
	{
	public:

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

		template<typename ComponentType>
		EnableIsBasedOf<BaseComponent, ComponentType> RegisterComponent();

	private:

		// This class will get execute almost every frame,
		// it makes sense that it would live on the stack with WorldEntity
		InputSettings m_InputSettings;

		SharedPtr<SimpleThreadPool> m_SimpleThreadPool;

		// -------- ECS ----------

		// Has loop iteration, need to take care of run-time write
		SystemPipeline m_SystemPipeline;
		LFStack<SharedPtr<SystemBase>> m_PendingAddSystem;

		//LFQueue<SharedPtr<SystemBase>> m_PendingAddSystem;

		// No loop iteration
		// Save data when component first created
		std::unordered_map<ComponentTypeId, SharedPtr<BaseComponent>> m_ComponentMap;
		
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
}


