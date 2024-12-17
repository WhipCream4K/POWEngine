#pragma once


#include "RenderPass.h"
#include "Viewport.h"
#include "Core/IModule.h"
#include "RenderGraph.h"

namespace powe
{
    class SceneRenderEvent;
    class RenderPass;
    class RenderContext;
    class SceneRenderGraph;
    class SceneRenderer : public IModule
    {
        
    public:

        SceneRenderer();
        
        ~SceneRenderer() override;

        void OnCreate(ModulesManager* manager) override;
        void OnExit(ModulesManager*) override {}
        void SetRenderContext(RenderContext* context) noexcept { m_RenderContext = context; }
        SceneRenderGraph& GetSceneRenderGraph() { return m_SceneRenderGraph; }
        void Draw();
        
    private:

        void Run();
        
        SceneRenderGraph m_SceneRenderGraph;
        SharedPtr<SceneRenderEvent> m_SceneRenderEvent;

        // RenderContext would likely outlive SceneRenderer
        // and be managed by Application
        RenderContext* m_RenderContext;

        std::atomic_flag m_RenderFlag;
        std::jthread m_RenderThread;
        std::atomic_bool m_ThreadStop;
        
    };
}

