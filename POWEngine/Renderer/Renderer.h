#pragma once

#include <functional>
#include <queue>
#include <semaphore>

#include "RenderPass.h"
#include "Viewport.h"
#include "Utils/Utils.h"


namespace powe
{
    class RenderPass;
    class RenderContext;
    class Window;
    class BaseRenderSystem;
    
    class Renderer
    {
        
    public:

        Renderer(size_t renderBufferCount = 2,PMRResource* memResource);
        ~Renderer();
        
        void Render(Window* window);
        
        void AddPass(std::string_view passName,std::function<void(RenderContext&)> func,
            Vector<std::string> dependencies = {}, Viewport::Flag flag = Viewport::Flag::None);

        void RemovePass(std::string_view passName);
        
    private:

        void Run();

        void PassResolve(RenderPass* pass);

        Vector<RenderPass> m_RenderPasses;

        RenderContext* m_ActiveContext;
        Vector<SharedPtr<RenderContext>> m_RenderContexts;
        uint8_t m_ActiveContextIndex;

        // TODO: Maybe we should have a render system for each window
        Window* m_ActiveWindow;
        
        std::mutex m_RenderQueueMutex;
        std::condition_variable m_RenderCV;
        std::atomic_flag m_RenderFlag;
        std::jthread m_RenderThread;
        std::atomic_bool m_ThreadStop;
        
        PMRResource* m_DefaultAllocator;
        
    };
}

