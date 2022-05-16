#include "pch.h"
#include "Renderer.h"

#include "RendererImpl.h"
#include "POWEngine/Window/Window.h"

#if USE_SFML_RENDERER
#include "SFML/SFML2DRenderer.h"
using RendererType = powe::SFML2DRenderer;
#endif

powe::Renderer::Renderer(uint32_t width, uint32_t height, const OtherWindowParams& other)
	: m_RendererImpl(std::make_unique<RendererType>(width,height,other))
{
}

powe::Renderer::~Renderer() = default;

void powe::Renderer::PushRenderBuffer(RawByte* sourceAddress, uint32_t count)
{
	
}

void powe::Renderer::DrawBufferOnWindow(const Window& window) const
{
	m_RendererImpl->DrawBufferOnWindow(window);
}

