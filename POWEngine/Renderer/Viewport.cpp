#include "pch.h"
#include "Viewport.h"

#include "RenderContext.h"
#include "Renderer/RenderTarget.h"

void powe::Viewport::Present(RenderContext& context) const
{
    m_RenderTarget->Present(context);
}

void powe::Viewport::Clear(RenderContext& ctx) const
{
    m_RenderTarget->Clear(ctx);
}
