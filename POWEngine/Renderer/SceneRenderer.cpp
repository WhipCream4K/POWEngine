#include "pch.h"
#include "SceneRenderer.h"

#include "RenderContext.h"
#include "RenderPass.h"
#include "Core/ModulesManager.h"
#include "Utils/Utils.h"
#include "Core/WindowManager.h"
#include "Core/Application/Application.h"

powe::SceneRenderer::SceneRenderer(size_t renderBufferCount, PMRResource* memResource)
    : IModule("SceneRenderer")
      , m_RenderPasses(memResource)
      , m_ActiveContext(nullptr)
      , m_RenderContexts(memResource)
      , m_ActiveContextIndex(0)
      , m_RenderFlag(ATOMIC_FLAG_INIT)
      , m_ThreadStop(false)
      , m_DefaultAllocator(memResource)
{
    m_RenderContexts.reserve(renderBufferCount);
}

powe::SceneRenderer::~SceneRenderer()
{
    m_ThreadStop = true;
    m_RenderFlag.test_and_set(std::memory_order_acquire);
    m_RenderFlag.notify_one();
}

void powe::SceneRenderer::OnStartUp(ModulesManager* manager)
{
    manager->AddModuleDependencies("Renderer", {"CoreEngine"});
}

void powe::SceneRenderer::OnUpdate(float)
{
    m_ActiveContextIndex = (m_ActiveContextIndex + 1) % m_RenderContexts.size();
    m_ActiveContext = m_RenderContexts[m_ActiveContextIndex].get();
    m_RenderFlag.test_and_set(std::memory_order_acquire);
    m_RenderFlag.notify_one();
}

void powe::SceneRenderer::AddPass(std::string_view passName, std::function<void(RenderContext&)> func,
                                  Vector<std::string> dependencies, Viewport::Flag flag)
{
    // Resolve dependencies of viewport render pass
    // for example, if we have a pass that requires a depth buffer, depth buffer has to finished rendering before
    // this pass can start rendering
}

void powe::SceneRenderer::RemovePass(std::string_view passName)
{
    auto findItr = std::ranges::find_if(m_RenderPasses,
                                        [passName](const auto& pass) { return pass.GetName() == passName; });
    if (findItr != m_RenderPasses.end())
    {
        m_RenderPasses.erase(findItr);
    }
}

void powe::SceneRenderer::Run()
{
    while (!m_ThreadStop)
    {
        m_RenderFlag.wait(true, std::memory_order_relaxed);

        if (m_ThreadStop) break;

        // Is Able to Render

        

        for (auto& pass : m_RenderPasses)
        {
            pass.Execute(*m_ActiveContext);
        }

        m_SceneViewport->Present(*m_ActiveContext);

        m_RenderFlag.clear(std::memory_order_release);
    }
}

void powe::SceneRenderer::PassResolve(RenderPass* pass)
{
}
