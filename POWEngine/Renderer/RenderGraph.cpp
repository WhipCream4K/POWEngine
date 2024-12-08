#include "pch.h"
#include "RenderGraph.h"
#include "RenderPass.h"

powe::SceneRenderGraph::SceneRenderGraph(uint32_t)
{
}

void powe::SceneRenderGraph::Execute(RenderContext& ctx)
{
    ResolvePass();

    for(auto& pass : m_RenderPasses)
    {
        pass->Execute(ctx, *this);
    }
}

void powe::SceneRenderGraph::ResolvePass()
{
    
}

void powe::SceneRenderGraph::AddPass(UniquePtr<RenderPass>&& pass)
{
    m_RenderPasses.push_back(std::move(pass));
}