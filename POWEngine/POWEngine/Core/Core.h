#pragma once
#include <future>

#include "POWEngine/Window/WindowContext.h"
//#include "POWEngine/Rendering/Renderer.h"

namespace powe
{
	class InputManager;
	class RenderAPI;
	class ServiceLocator;
	class Window;
	class WorldEntity;
	class WorldClock;
	class Renderer;
	class Core final
	{
	public:

		Core();

		bool TranslateWindowInputs(
			const Window& window,
			WorldEntity& worldEntt
		) const;

		/**
		 * \brief Call this function before entering the core loop to prevent a big deltaTime in the first frame
		 */
		void StartWorldClock() const;

		void Step(WorldEntity& worldEntity);

		WorldClock& GetWorldClock() const { return *m_WorldClock; }
		Renderer& GetRenderer() const { return *m_MainRenderer; }

		void Draw(const Window& window, const WorldEntity& worldEntt) const;

		void RegisterRendererType(OwnedPtr<RenderAPI>&& renderAPI) const;

		~Core();

		Core(const Core&) = delete;
		Core& operator=(const Core&) = delete;
		Core(Core&&) = default;
		Core& operator=(Core&&) = default;

	private:

		/**
		 * \brief Implementation of the core engine to expand the scalability of the core engine
		 * i.e Inputs handle by sdl and rendering handles by OpenGL or DX12
		 */
		 //OwnedPtr<CoreImpl> m_CoreImpl;

		OwnedPtr<WorldClock> m_WorldClock;
		OwnedPtr<Renderer> m_MainRenderer;
		OwnedPtr<InputManager> m_InputManager{};

	};
}


