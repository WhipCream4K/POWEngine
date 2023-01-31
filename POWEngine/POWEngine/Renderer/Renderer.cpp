#include "pch.h"
#include "Renderer.h"

#include "POWEngine/Renderer/System/RenderSystemBase.h"
#include "NullRenderer.h"
#include "POWEngine/Core/GameObject/GameObject.h"
#include "POWEngine/Debug/imgui/SFMLImGuiOverlay.h"

powe::Renderer::Renderer()
    : m_RenderAPI(std::make_unique<NullRenderer>())
{
#if USE_IMGUI && USE_SFML_RENDERER
#else
    // m_DebugOverlay = std::make_unique<NullRenderer>();
#endif

    m_DebugOverlay = std::make_unique<SFMLImGuiOverlay>();

}

powe::Renderer::~Renderer() = default;

powe::RenderAPI* powe::Renderer::GetRenderAPI() const
{
    return m_RenderAPI.get();
}

void powe::Renderer::Update(float deltaTime) const
{
    m_DebugOverlay->Update(deltaTime);
}

void powe::Renderer::SetTargetWindow(const SharedPtr<Window>& window)
{
    m_TargetWindow = window;
    m_RawTargetWindow = window.get();
    
    if(!m_IsInitialized)
    {
        m_RenderAPI->OnWindowCreate(*window);
        m_DebugOverlay->OnWindowCreate(*window);
        m_IsInitialized = true;
    }
    else
    {
        m_RenderAPI->OnChangeWindow(*window);
        m_DebugOverlay->OnChangeWindow(*window);
    }

    m_RawTargetWindow->RemoveOnResizeCallback(m_RendererResizeCallbackHandle);
    m_RawTargetWindow->RemoveOnResizeCallback(m_DebugResizeCallbackHandle);
    
    m_RendererResizeCallbackHandle = window->RegisterOnResizeCallback([this](const Window& window)
    {
        m_RenderAPI->OnWindowResize(window);
    });

    m_DebugResizeCallbackHandle = window->RegisterOnResizeCallback([this](const Window& window)
    {
        m_DebugOverlay->OnWindowResize(window);
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

void powe::Renderer::SetClearColor(const glm::uvec4& )
{
    
}

void powe::Renderer::ClearBackBuffer() const
{
    m_DebugOverlay->ClearBackBuffer();
    m_RenderAPI->ClearBackBuffer();
}

void powe::Renderer::DisplayBuffer() const
{
    m_DebugOverlay->DisplayBuffer(*m_RawTargetWindow);
    m_RenderAPI->DisplayBuffer(*m_RawTargetWindow);
}

void powe::Renderer::DeferredDrawOnWindow() const
{
    m_RenderAPI->DeferredDrawOnWindow(*m_RawTargetWindow);
}
