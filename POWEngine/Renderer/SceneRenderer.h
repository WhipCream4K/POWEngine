#pragma once

#include <future>

#include "RenderPass.h"
#include "Viewport.h"
#include "Core/IModule.h"
#include "RenderGraph.h"

namespace powe
{
    class RenderPass;
    class RenderContext;
    class Window;
    class BaseRenderSystem;
    class WindowManager;
    class SceneRenderGraph;
    class SceneRenderer : public IModule
    {
        
    public:

        SceneRenderer(size_t renderBufferCount = 2);
        
        ~SceneRenderer() override;

        void OnStartUp(ModulesManager* manager) override;
        
        SceneRenderGraph& GetSceneRenderGraph() { return m_SceneRenderGraph; }

        std::future<void> Render();
        
    private:

        void Run();
        
        // SceneRender needs render interface to exist for it to function hence shared ptr
        SharedPtr<RenderContext> m_RenderContext;
        SceneRenderGraph m_SceneRenderGraph;
        
        std::atomic_flag m_RenderFlag;
        std::jthread m_RenderThread;
        std::promise<void> m_RenderPromise;
        std::atomic_bool m_ThreadStop;
        
    };
}

