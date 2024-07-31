#pragma once

#include "Core/Core.h"
#include "Renderer/RenderAPI.h"
#include "AppDesc.h"
#include "Core/Memory/Allocator.h"

namespace powe
{
    class WorldEntity;
    class Window;
    class Core;
    class Layer;
    class Clock;
    class WindowManager;
    class Application
    {
    public:

        Application(const AppDesc& desc);
        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;
        Application(Application&&) = delete;
        Application& operator=(Application&&) = delete;
        virtual ~Application() = default;

        void PushLayer(UniquePtr<Layer>&& layer);
        void PopLayer();
        void Run();

    private:

        Vector<UniquePtr<Layer>> m_LayerStack;
        UniquePtr<Clock> m_Clock;
        UniquePtr<WindowManager> m_WindowManager;

        TrackableAllocator m_TrackAllocator;
    };
}
