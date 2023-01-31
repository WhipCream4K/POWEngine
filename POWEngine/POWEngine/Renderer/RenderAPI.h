#pragma once

#include "POWEngine/Window/Window.h"

namespace powe
{
	class Window;
	class RenderAPI
	{
	public:
		
		virtual void DisplayBuffer(const Window&) = 0;
		virtual void DeferredDrawOnWindow(const Window&) {}
		
		virtual void OnWindowCreate(const Window&) {}
		virtual void OnWindowResize(const Window&) {}
		virtual void OnChangeWindow(const Window&) {}
		virtual void OnWindowClose(const Window&) {}
		
		virtual void SetClearColor(const glm::uvec4& ) {}
		virtual void ClearBackBuffer() = 0;
		
	public:

		RenderAPI();
		RenderAPI(const RenderAPI&) = delete;
		RenderAPI& operator=(const RenderAPI&) = delete;
		RenderAPI(RenderAPI&&) = delete;
		RenderAPI& operator=(RenderAPI&&) = delete;
		virtual ~RenderAPI();
	};
}


