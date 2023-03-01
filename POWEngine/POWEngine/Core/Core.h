#pragma once
#include <future>

#include "ICore.h"
#include "POWEngine/Renderer/Renderer.h"
#include "POWEngine/Window/Window.h"

namespace powe
{
	class InputManager;
	class RenderAPI;
	class ServiceLocator;
	class Window;
	class WorldEntity;
	class WorldClock;
	class Renderer;
	class Core final : public ICore
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

		
		void Step(WorldEntity& worldEntity) const;
		void Draw(const Renderer& renderer,WorldEntity& world) const;
		
		bool FullStepMultiThreaded(const Renderer& renderer,WorldEntity& world);
		bool FullStep(const Renderer& renderer,WorldEntity& world) const;

		
		Core(const Core&) = delete;
		Core& operator=(const Core&) = delete;
		Core(Core&&) noexcept = default;
		Core& operator=(Core&&) = default;
		~Core();

	private:

		void WaitForLastFrameDisplay(const Renderer& renderer) const;
		
		static constexpr uint32_t MaxQueuedFrame{2};
		uint32_t m_RenderCommandCount{};

		uint32_t m_CurrentFrameCount{};
		
		bool m_StartFrame{};
		
		/**
		 * \brief Implementation of the core engine to expand the scalability of the core engine
		 * i.e Inputs handle by sdl and rendering handles by OpenGL or DX12
		 */

	};
}


