#pragma once

//#include "RendererImpl.h"
#include "RenderAPI.h"

namespace powe
{
	class NullRenderer : public RenderAPI
	{
	public:

		// NullRenderer(uint32_t width, uint32_t height, const OtherWindowParams& other);
		NullRenderer() = default;
		void DeferredDrawOnWindow(const Window&) override {}
		void DisplayBuffer(const Window&) override {}
		void ClearBackBuffer() override {}
	};
}


