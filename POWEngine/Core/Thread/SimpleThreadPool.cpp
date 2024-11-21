#include "pch.h"
#include "Core/Application/Application.h"
#include "SimpleThreadPool.h"

powe::SimpleThreadPool::SimpleThreadPool(size_t threadCount)
	: m_ThreadCV()
	, m_Mutex()
	, m_Stop(false)
{
	
	MemoryManager* memManager{ Application::GetAppService<MemoryManager>() };
	if(memManager)
	{
		PMRResource* appResource{memManager->GetAllocator("Application")};;

		m_Workers = Vector<std::jthread>{appResource};

		for (size_t i = 0; i < threadCount; ++i) {
			m_Workers.emplace_back([this] { Run(); });
		}
	}
}

powe::SimpleThreadPool::~SimpleThreadPool()
{
	{
		std::scoped_lock lock(m_Mutex);
		m_Stop = true;
	}

	m_ThreadCV.notify_all();
	for (auto& worker : m_Workers) {
		worker.join();
	}
}

void powe::SimpleThreadPool::Run()
{
	while (true) {

		std::packaged_task<void()> task;

		{
			std::unique_lock lock(m_Mutex);

			m_ThreadCV.wait(lock, [this] {return m_Stop || !m_Tasks.empty(); });
			if (m_Stop && m_Tasks.empty()) return;
			task = std::move( m_Tasks.front() );
			m_Tasks.pop();

		}

		task();
	}
}

powe::PMRResource* powe::SimpleThreadPool::GetResource() const
{
	// Warning could throw
	return Application::GetAppService<MemoryManager>()->GetAllocator("Application");
}
