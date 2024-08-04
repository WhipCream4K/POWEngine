#pragma once

#include <typeindex>

#include "ECS/Archetype.h"
#include "EngineLayer.h"
#include "SceneSystem.h"
#include "Utils/Utils.h"


namespace powe
{
	class ECSManager;
	class EngineLayer;
	class Scene final
	{
	public:

		Scene(EngineLayer* parent);

		void OnStart();
		void OnExit();
		void OnWindowEvents(WindowManager::Events& events);
		void Update(float deltaTime);

		template<CSceneSystem T>
		T* AddSceneSystem(T&& system)
		{
			auto sceneSystem = AllocateUnique<T>(m_EngineLayer->GetAllocator(),std::move(system));
			m_SceneSystems.push_back(std::move(sceneSystem));
			return static_cast<T*>(m_SceneSystems.back().get());
		}

		template<typename T> requires CSceneSystem<T>
		void RemoveSceneSystem()
		{
			std::erase_if(m_SceneSystems, [](const UniquePtr<SceneSystem>& sceneSystem)
				{
					return typeid(*sceneSystem.get()) == typeid(T);
				});
		}

		ECSManager& GetECSManager() const { return *m_ECSManager.get(); }

		// EngineLayer is guaranteed to be valid as long as Scene Exists
		EngineLayer& GetParentLayer() const { return m_EngineLayer.get(); }

		

	private:

		UniquePtr<ECSManager> m_ECSManager;
		RefWrap<EngineLayer> m_EngineLayer;
		Vector<UniquePtr<SceneSystem>> m_SceneSystems;
		UnOrderedMap<std::type_index, SharedPtr<void>> m_SubSystems;
	};
}


