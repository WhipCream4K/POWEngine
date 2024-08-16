#include "pch.h"
#include "Renderer.h"

powe::Renderer::Renderer(size_t renderBufferCount, PMRResource* memResource)
    : m_RenderSystems(memResource)
      , m_Frames(memResource)
      , m_ActiveFrame()
      , m_DefaultAllocator()
{
    m_Frames.resize(renderBufferCount);
}

void powe::Renderer::BeginFrame()
{
    m_Frames[m_ActiveFrame].Clear();
}

void powe::Renderer::EndFrame()
{
    m_ActiveFrame = (m_ActiveFrame + 1) % m_Frames.size();
}

void powe::Renderer::Present(Window& window)
{
    
}

void powe::Renderer::Run()
{
    while (true)
    {
        
    }
}
