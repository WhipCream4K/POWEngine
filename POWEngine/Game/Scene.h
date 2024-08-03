#pragma once

#include "ECS/Archetype.h"
#include "EngineLayer.h"
#include "SceneSystem.h"

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
		void Update(float deltaTime);

		template<CSceneSystem T>
		T* AddSceneSystem(SceneSystem&& system);

		template<typename T>
		void RemoveSceneSystem()
		{
			std::erase_if(m_SceneSystems, [](const UniquePtr<SceneSystem>& sceneSystem)
				{
					return typeid(*sceneSystem.get()) == typeid(T);
				});
		}

		ECSManager& GetECSManager() { return *m_ECSManager.get(); }

		// EngineLayer is guaranteed to be valid as long as Scene Exists
		EngineLayer& GetParentLayer() { return *m_EngineLayer; }

	private:

		UniquePtr<ECSManager> m_ECSManager;
		EngineLayer* m_EngineLayer; // Scene doesn't own EngineLayer
		Vector<UniquePtr<SceneSystem>> m_SceneSystems;
	};
	template<CSceneSystem T>
	inline T* Scene::AddSceneSystem(SceneSystem&& system)
	{
		m_SceneSystems.emplace_back(AllocateUnique<T>(std::move(system), m_EngineLayer->GetAllocator()));
		T* scenePtr{ m_SceneSystems.back().get() };
		scenePtr->OnInitialize();
		return static_cast<T*>(scenePtr);
	}
}


