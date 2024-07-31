#pragma once

#include "Core/CustomTypes.h"
#include "Core/Layer.h"

namespace powe
{
	class Scene;
	class EngineLayer : public Layer
	{
	public:

		Scene& CreatScene(std::string_view sceneName);

	protected:

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(float deltaTime) override;

	private:

		//Vector<UniquePtr<Scene>> m_Scenes
		UnOrderedMap<std::string,UniquePtr<Scene>> m_Scenes;
	};
}