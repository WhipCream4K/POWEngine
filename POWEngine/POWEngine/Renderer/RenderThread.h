#pragma once
#include <future>
#include <queue>

#include "POWEngine/ECS/SparseComponentManager.h"
#include "RenderCommand.h"

namespace powe
{
    class RenderCommandBase;
    class Window;
    
    // Thread - One producer, One consumer
    class RenderThread final
    {
    public:

        RenderThread();
        ~RenderThread();

        template<typename T>
        EnableIsBasedOf<RenderCommandBase,T> IssueRenderCommandToGameWindow(T command);
        
    private:
        
        void Run();
        
        std::future<void> m_ThreadWorker{};
        std::condition_variable m_ThreadCV{};
        
        std::mutex m_Mutex{};
        std::queue<std::packaged_task<void()>> m_RenderTasks{};
        
        SharedPtr<RenderAPI> m_Renderer{};
    };

    template <typename T>
    EnableIsBasedOf<RenderCommandBase, T> RenderThread::IssueRenderCommandToGameWindow(T command)
    {
        std::packaged_task<void()> task{[this,command]()
        {
            // TODO: Get window from application
            Window* some{};
            RenderCommandBase* rcBase{static_cast<RenderCommandBase*>(&command)};
            rcBase->Render(*some,*m_Renderer);
        }};

        {
            std::scoped_lock lock{m_Mutex};
            m_RenderTasks.push(std::move(task));
        }

        m_ThreadCV.notify_one();
    }
}
