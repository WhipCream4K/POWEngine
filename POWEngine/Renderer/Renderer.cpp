#include "pch.h"
#include "Renderer.h"

powe::Renderer::Renderer(PMRResource* memResource)
{
    m_RenderThread = std::jthread([this] { Run(); });
}

void powe::Renderer::Initialize()
{
}

void powe::Renderer::Run()
{
    
}
