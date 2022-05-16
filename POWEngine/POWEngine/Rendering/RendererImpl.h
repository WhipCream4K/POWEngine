#pragma once
#include "POWEngine/Window/WindowContext.h"

namespace powe
{
	class Window;
	class RendererImpl
	{
	public:

		RendererImpl(uint32_t width, uint32_t height, const OtherWindowParams& other);
		RendererImpl(const RendererImpl&) = delete;
		RendererImpl& operator=(const RendererImpl&) = delete;
		RendererImpl(RendererImpl&&) = delete;
		RendererImpl& operator=(RendererImpl&&) = delete;
		virtual ~RendererImpl() = default;

		virtual void PushRenderBuffer(RawByte* buffer, uint32_t size) = 0;
		virtual void DrawBufferOnWindow(const Window& window) = 0;

	private:


	};
}



