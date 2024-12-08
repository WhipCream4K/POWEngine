#include "pch.h"
#include "Core/Application/Application.h"
#include "Core/ModulesManager.h"
#include "SimpleThreadPool.h"

using namespace powe;

SimpleThreadPool::SimpleThreadPool(uint32_t threadCount)
	: IModule("SimepleThreadPool")
	, m_ThreadCount(threadCount)
	, m_ThreadCV()
	, m_Mutex()
	, m_Stop(false)
{
	const AllocatorContext context{AllocatorScope::ThreadPool};
	auto* upStream{context.GetResource()};
	
	m_Workers = Vector<std::jthread>{upStream};
}

void SimpleThreadPool::OnCreate(ModulesManager*)
{
	for (uint32_t i = 0; i < m_ThreadCount; ++i) {
		m_Workers.emplace_back([this] { Run(); });
	}
}

SimpleThreadPool::~SimpleThreadPool()
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

void SimpleThreadPool::Run()
{
	while (true) {

		std::packaged_task<void()> task{};

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
