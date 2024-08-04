#pragma once
#include "Core/WindowManager.h"

namespace powe
{
	class Scene;
	class SceneSystem
	{
	public:

		SceneSystem() = default;
		SceneSystem(const SceneSystem&) = delete;
		SceneSystem& operator=(const SceneSystem&) = delete;
		SceneSystem(SceneSystem&&) = delete;
		SceneSystem& operator=(SceneSystem&&) = delete;
		virtual ~SceneSystem() = default;

		virtual void OnInitialize() = 0;
		virtual void OnStart(Scene&) = 0;
		virtual void OnUpdate(Scene&,float) = 0;
		virtual void OnExit(Scene&) = 0;
	};

	template<typename T>
	concept CSceneSystem = std::is_base_of_v<SceneSystem, T>;
}
