#pragma once

#include "Core/CustomTypes.h"
#include "Core/Layer.h"
#include "Core/Memory/Allocator.h"

namespace powe
{
	class Scene;
	class EngineLayer : public Layer
	{
	public:

		EngineLayer();

		Scene& CreatScene(std::string_view sceneName);
		void SetActiveScene(std::string_view sceneName) { m_ActiveScene = m_Scenes.at(sceneName.data()).get(); }
		std::pmr::memory_resource* GetAllocator() { return &m_TrackAllocator; }

	protected:

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(float deltaTime) override;

	private:
		TrackableAllocator m_TrackAllocator;

		UnOrderedMap<std::string,UniquePtr<Scene>> m_Scenes;
		Scene* m_ActiveScene;
	};
}