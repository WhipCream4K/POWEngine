#pragma once

#include "RendererImpl.h"

namespace powe
{
	class NullRenderer : public RendererImpl
	{
	public:

		NullRenderer(uint32_t width, uint32_t height, const OtherWindowParams& other);
		void PushRenderBuffer(RawByte* buffer, uint32_t size) override {}
		void DrawBufferOnWindow(const Window& window) override {}
	};
}


