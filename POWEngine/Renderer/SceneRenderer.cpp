#include "pch.h"
#include "SceneRenderer.h"

#include "RenderContext.h"
#include "RenderPass.h"
#include "Core/ModulesManager.h"

powe::SceneRenderer::SceneRenderer()
    : IModule("SceneRenderer")
    , m_RenderContext()
    , m_SceneRenderGraph()
    , m_RenderFlag()
    , m_ThreadStop(false)
{
}

powe::SceneRenderer::~SceneRenderer()
{
    m_ThreadStop = true;
    m_RenderFlag.test_and_set(std::memory_order_acquire);
    m_RenderFlag.notify_one();
}

void powe::SceneRenderer::OnCreate(ModulesManager*)
{
}

void powe::SceneRenderer::Draw()
{
    m_RenderFlag.test_and_set(std::memory_order_acquire);
}

void powe::SceneRenderer::Run()
{
    while (!m_ThreadStop)
    {
        m_RenderFlag.wait(true, std::memory_order_relaxed);

        if (m_ThreadStop) break;

        m_SceneRenderGraph.Execute(*m_RenderContext);
        
        m_RenderFlag.clear(std::memory_order_release);
    }
}
