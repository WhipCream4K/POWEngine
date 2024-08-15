#pragma once

#include <functional>
#include <queue>

namespace powe
{
    class BaseRenderSystem;
    class Renderer
    {
    public:

        Renderer(PMRResource* memResource = DefaultAllocator::Engine);
        void Initialize();
        void BeginFrame();
        void EndFrame();

    private:

        void Run();

        std::queue<std::function<void*()>> m_RenderQueue;
        Vector<std::pair<uint32_t, UniquePtr<BaseRenderSystem>>> m_RenderSystems;
        std::mutex m_RenderQueueMutex;
        std::condition_variable m_RenderQueueCV;
        std::jthread m_RenderThread;
    };
}

