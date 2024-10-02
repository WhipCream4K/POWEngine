#pragma once

#include "Core/CustomTypes.h"
#include "Core/Layer.h"
#include "Core/Memory/Allocator.h"

namespace powe
{
	class Scene;
	class Renderer;
	class EngineLayer final : public Layer
	{
	public:

		EngineLayer();

		Scene& CreatScene(std::string_view sceneName);
		void SetActiveScene(std::string_view sceneName) { m_ActiveScene = m_Scenes.at(sceneName.data()).get(); }

		static PMRResource* GetAllocator() { return DefaultAllocator::Engine; }

		Renderer& GetRenderer() const { return *m_Renderer; }

	protected:

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(float deltaTime) override;
		void OnWindowEvents(const Window::EventQueue& events) override;

	private:

		UnOrderedMap<std::string,UniquePtr<Scene>> m_Scenes;
		UniquePtr<Renderer> m_Renderer;
		PMRResource* m_DefaultAllocator;
		Scene* m_ActiveScene;
	};
}
