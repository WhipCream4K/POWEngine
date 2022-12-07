#pragma once
#include "POWEngine/Window/Window.h"
#include "POWEngine/Window/WindowContext.h"

namespace powe
{
	class Window;
	class RenderAPI
	{
	public:

		virtual void SetClearColor(const glm::uvec4& color) {}
		virtual void OnWindowCreate(const Window& window) {}
		virtual void DrawBufferOnWindow(const Window& window) const = 0;
		virtual void OnWindowResize(const Window& window) {}
		virtual void ClearBackBuffer() = 0;
		
	public:

		RenderAPI(uint32_t width,uint32_t height,const OtherWindowParams& window);
		RenderAPI() = default;
		RenderAPI(const RenderAPI&) = delete;
		RenderAPI& operator=(const RenderAPI&) = delete;
		RenderAPI(RenderAPI&&) = delete;
		RenderAPI& operator=(RenderAPI&&) = delete;
		virtual ~RenderAPI() = default;
	};
}


