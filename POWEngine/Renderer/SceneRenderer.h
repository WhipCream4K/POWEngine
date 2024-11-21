#pragma once

#include <functional>

#include "RenderPass.h"
#include "Viewport.h"
#include "Core/IModule.h"

namespace powe
{
    class RenderPass;
    class RenderContext;
    class Window;
    class BaseRenderSystem;
    class WindowManager;

    class SceneRenderer : public IModule
    {
        
    public:

        SceneRenderer(size_t renderBufferCount = 2);
        
        ~SceneRenderer() override;

        void OnStartUp(ModulesManager* manager) override;
        
        void OnUpdate(float deltaTime) override;
        
        void AddPass(std::string_view passName,std::function<void(RenderContext&)> func,
            Vector<std::string> dependencies = {}, RenderTarget::Flag = RenderTarget::None);

        void RemovePass(std::string_view passName);
        
    private:

        void Run();

        void PassResolve(RenderPass* pass);

        Vector<RenderPass> m_RenderPasses;

        RenderContext* m_ActiveContext;
        Vector<SharedPtr<RenderContext>> m_RenderContexts;
        uint8_t m_ActiveContextIndex;

        Viewport* m_SceneViewport;
        
        std::atomic_flag m_RenderFlag;
        std::jthread m_RenderThread;
        std::atomic_bool m_ThreadStop;
        
    };
}

