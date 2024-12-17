#include "pch.h"
#include "RenderGraph.h"
#include "RenderPass.h" 

void powe::SceneRenderGraph::Execute(RenderContext& ctx)
{
    m_Viewport->Bind(ctx);

    ResolvePass();

    for(auto& pass : m_RenderPasses)
    {
        pass->Execute(ctx, *this);
    }

    m_Viewport->Present(ctx);
}

void powe::SceneRenderGraph::ResolvePass()
{
    
}

void powe::SceneRenderGraph::AddPass(UniquePtr<RenderPass>&& pass)
{
    m_RenderPasses.push_back(std::move(pass));
}