#include "pch.h"


#include "SceneRenderer.h"
#include "Core/ModulesManager.h"
#include "SceneRenderEvent.h"


powe::SceneRenderer::SceneRenderer() : IModule("SceneRenderer"), m_RenderFlag(), m_ThreadStop(false)
{
    m_RenderThread = std::jthread(&powe::SceneRenderer::Run, this);
}

powe::SceneRenderer::~SceneRenderer()
{
    m_ThreadStop = true;
    m_RenderFlag.test_and_set(std::memory_order_acquire);
    m_RenderFlag.notify_one();
}

void powe::SceneRenderer::OnCreate(ModulesManager *modulesManager)
{
    auto resc{modulesManager->GetModuleResource<SceneRenderer>()};
    m_SceneRenderEvent = AllocateShared<SceneRenderEvent>(resc.get(), *this);

    Application::Get().RegisterAppEvent(m_SceneRenderEvent);
}

void powe::SceneRenderer::Draw()
{
    if(m_RenderFlag.test(std::memory_order_acquire))
        return;

    m_RenderFlag.test_and_set(std::memory_order_acquire);
    m_RenderFlag.notify_one();
}

void powe::SceneRenderer::Run()
{
    try
    {
        while (!m_ThreadStop)
        {
            m_RenderFlag.wait(true, std::memory_order_relaxed);

            if (m_ThreadStop)
                break;
            
            m_SceneRenderGraph.Execute(*m_RenderContext);

            m_RenderFlag.clear(std::memory_order_release);
        }
    }
    catch (std::exception &e)
    {
        powe::Error(e.what());
        throw e;
    }
}
