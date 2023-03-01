#include "pch.h"
#include "Renderer.h"

#include "POWEngine/Renderer/System/RenderSystemBase.h"
#include "NullRenderer.h"
#include "POWEngine/Core/GameObject/GameObject.h"
// #include "POWEngine/Debug/imgui/SFMLImGuiOverlay.h"

powe::Renderer::Renderer()
    : m_RenderAPI(std::make_unique<NullRenderer>())
{
    
}

powe::Renderer::~Renderer() = default;

powe::RenderAPI* powe::Renderer::GetRenderAPI() const
{
    return m_RenderAPI.get();
}

void powe::Renderer::SetTargetWindow(const SharedPtr<Window>& window)
{
    m_TargetWindow = window;
    m_RawTargetWindow = window.get();
    
    if(!m_IsInitialized)
    {
        m_RenderAPI->OnWindowCreate(*window);
        m_IsInitialized = true;
    }
    else
    {
        m_RenderAPI->OnChangeWindow(*window);
    }

    m_RawTargetWindow->RemoveOnResizeCallback(m_RendererResizeCallbackHandle);
    
    m_RendererResizeCallbackHandle = window->RegisterOnResizeCallback([this](const Window& window)
    {
        m_RenderAPI->OnWindowResize(window);
    });
}

powe::Window* powe::Renderer::GetTargetWindow() const
{
    if(const auto targetWindow = m_TargetWindow.lock())
        return targetWindow.get();
    return nullptr;
}

void powe::Renderer::SetRenderAPI(OwnedPtr<RenderAPI> renderAPI)
{
    m_RenderAPI = std::move(renderAPI);
}

void powe::Renderer::SetClearColor(const glm::uvec4& color) const
{
    m_RenderAPI->SetClearColor(color);
}

void powe::Renderer::ClearBackBuffer() const
{
    m_RenderAPI->ClearBackBuffer();
}

void powe::Renderer::DisplayBuffer() const
{
    m_RenderAPI->DisplayBuffer(*m_RawTargetWindow);
}

void powe::Renderer::DeferredDrawOnWindow() const
{
    m_RenderAPI->DeferredDrawOnWindow(*m_RawTargetWindow);
}
