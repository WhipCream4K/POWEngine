#include "pch.h"
#include "RenderThread.h"

powe::RenderThread::RenderThread()
{
    m_ThreadWorker = std::async(std::launch::async,&RenderThread::Run,this);
}

powe::RenderThread::~RenderThread()
{
    {
        std::unique_lock lock{ m_Mutex };
        m_RenderTasks.push({});
    }

    m_ThreadCV.notify_all();
}

void powe::RenderThread::Run()
{
    while (true)
    {
        std::packaged_task<void()> task{};

        {
            std::unique_lock lock{ m_Mutex };

            m_ThreadCV.wait(lock, [this]()
                {
                    return !m_RenderTasks.empty();
                });

            task = std::move(m_RenderTasks.front());
            m_RenderTasks.pop();
        }

        if (!task.valid()) return;

        task();
    }
}
