#include "pch.h"
#include "SceneViewport.h"

#include "Renderer/RenderContext.h"


using namespace powe;

void SceneViewport::Bind(RenderContext& context) 
{
    // could be null use with caution
    context.SetViewport(*this);
    m_WindowViewport->Bind(context);
}

void SceneViewport::Present(RenderContext& context)
{
    // could be null use with caution
    context.SetViewport(*this);
    m_WindowViewport->Present(context);
}

void SceneViewport::BindWindow(Window* window) noexcept
{
    m_BindWindow = window;
    m_WindowViewport = window->GetViewport().get();
}