#pragma once

#include <functional>
#include <queue>
#include "RenderSystem.h"
#include "Frame.h"
#include "Utils/Utils.h"

namespace powe
{
    class Window;
    class BaseRenderSystem;
    class Renderer
    {
    public:

        Renderer(size_t renderBufferCount = 2,PMRResource* memResource = DefaultAllocator::Engine);
        
        void BeginFrame();
        void EndFrame();

        template<typename T,typename... Args> requires std::is_base_of_v<BaseRenderSystem,T>
        RenderSystem<T>& CreateRenderSystem(Args&&... args);

        // We can only present one context at a time
        void Present(Window& window);
        
    private:

        void Run();


        // I trust that there are minimal render system exist in one instance
        // so using vector find will always outweigh the benefits of using unordered_map
        Vector<std::pair<uint32_t, UniquePtr<BaseRenderSystem>>> m_RenderSystems;

        Vector<Frame> m_Frames;
        uint32_t m_ActiveFrame;
        
        std::queue<std::function<void*()>> m_RenderQueue;
        std::mutex m_RenderQueueMutex;
        std::condition_variable m_RenderQueueCV;
        std::jthread m_RenderThread;
        PMRResource* m_DefaultAllocator;
    };

    template <typename T, typename ... Args> requires std::is_base_of_v<BaseRenderSystem, T>
    RenderSystem<T>& Renderer::CreateRenderSystem(Args&&... args)
    {
        UniquePtr<BaseRenderSystem> renderSyste{AllocateUnique<T>(m_DefaultAllocator,std::forward<Args>(args)...)};
        m_RenderSystems.emplace_back(std::make_pair(renderSyste->GetRenderEntityID(),std::move(renderSyste));
        return *static_cast<RenderSystem<T>*>(m_RenderSystems.back().second.get());
    }

}

