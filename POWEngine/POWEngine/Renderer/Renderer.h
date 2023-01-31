#pragma once

#include "POWEngine/Window/WindowContext.h"
#include "POWEngine/Core/CustomTypes.h"

namespace powe
{
	class SFMLImGuiOverlay;
}

namespace powe
{
	struct Archetype;
	class RendererImpl;
	class Window;
	class RenderAPI;
	class WorldEntity;

	// a class that couple render api and target window together
	// because the renderer needs window to be alive during thread execution
	// hence the ownership has to take place
	class Renderer final
	{
	public:

		Renderer();
		~Renderer();
		

		/**
		 * \brief Renderer can change any window and it will reinitialize their buffer respectively
		 * \param window Target Window
		 */
		void SetTargetWindow(const SharedPtr<Window>& window);
		Window* GetUnCheckedTargetWindow() const {return m_RawTargetWindow;}
		Window* GetTargetWindow() const;
		void SetRenderAPI(OwnedPtr<RenderAPI> renderAPI);
		RenderAPI* GetRenderAPI() const;

		void Update(float deltaTime) const;
		void SetClearColor(const glm::uvec4& color);
		void ClearBackBuffer() const;
		void DisplayBuffer() const;
		void DeferredDrawOnWindow() const;
		

	private:
		WeakPtr<Window> m_TargetWindow;
		Window* m_RawTargetWindow;
		
		OwnedPtr<RenderAPI> m_RenderAPI;
		uint32_t m_RendererResizeCallbackHandle{};

		OwnedPtr<SFMLImGuiOverlay> m_DebugOverlay{};
		uint32_t m_DebugResizeCallbackHandle{};
		
		bool m_IsInitialized{};
	};
}

