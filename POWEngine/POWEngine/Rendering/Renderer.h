#pragma once
#include "POWEngine/Window/WindowContext.h"
#include "POWEngine/Core/CustomTypes.h"

namespace powe
{
	class RendererImpl;
	class Window;
	class Renderer
	{
	public:

		Renderer(uint32_t width, uint32_t height,const OtherWindowParams& other = {});
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator=(Renderer&&) = delete;
		~Renderer();

		void PushRenderBuffer(RawByte* sourceAddress, uint32_t count);
		void DrawBufferOnWindow(const Window& window) const;

	private:

		OwnedPtr<RendererImpl> m_RendererImpl;

	};
}

