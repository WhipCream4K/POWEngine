#pragma once

#include <functional>
#include <queue>

namespace powe
{
    class Renderer
    {
    public:

        void Initialize();
        void BeginFrame();
        void EndFrame();

    private:

        void Run();

        std::queue<std::function<void*()>> m_RenderQueue;
        std::jthread m_RenderThread;
    };
}

