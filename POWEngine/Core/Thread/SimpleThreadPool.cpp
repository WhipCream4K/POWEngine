#include "pch.h"
#include "SimpleThreadPool.h"

powe::SimpleThreadPool::SimpleThreadPool(std::pmr::memory_resource* memResource, size_t threadCount)
	: m_Workers(memResource)
	, m_MemResource(memResource)
	, m_Stop(false)
	, m_ThreadCV()
	, m_Mutex()
{
	for (size_t i = 0; i < threadCount; ++i) {
		m_Workers.emplace_back([this] { Run(); });
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
