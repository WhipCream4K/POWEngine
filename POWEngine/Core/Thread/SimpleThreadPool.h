
#pragma once

#include <thread>
#include <future>
#include <queue>

#include "Core/CustomTypes.h"
#include "Core/IModule.h"

namespace powe
{
	class SimpleThreadPool : public IModule
	{
	public:

		SimpleThreadPool(size_t threadCount = (size_t)std::thread::hardware_concurrency());
		SimpleThreadPool(const SimpleThreadPool&) = delete;
		SimpleThreadPool& operator=(const SimpleThreadPool&) = delete;
		SimpleThreadPool(SimpleThreadPool&&) noexcept = delete;
		SimpleThreadPool& operator=(SimpleThreadPool&&) noexcept = delete;
		~SimpleThreadPool();

		void OnStartUp(ModulesManager*) override;
		void OnExit(ModulesManager*) override { }

	public:

		template<typename Func, typename ... Args>
		auto EnqueueReturn(Func&& fn, Args&&... args) -> std::future<decltype(fn(args...))>
		{
			using Ret = std::invoke_result_t<Func, Args...>;

			if(m_Stop)
				return;

			auto task = std::allocate_shared<std::packaged_task<Ret()>>(GetResource(), [func = std::forward<Func>(fn), ...iArgs = std::forward<Args>(args)]() mutable {
				return func(iArgs...);
				});

			std::future<Ret> res = task->get_future();

			{
				std::scoped_lock lock(m_Mutex);
				m_Tasks.emplace([task] { (*task)(); });
			}

			m_ThreadCV.notify_one();

			return res;
		}

		template<typename Func, typename ... Args>
		void EnqueueDetach(Func&& fn, Args&&... args)
		{

			if (m_Stop)
				return;

			auto task = std::allocate_shared<std::packaged_task<void()>>(GetResource(), [func = std::forward<Func>(fn), ...iArgs = std::forward<Args>(args)]() mutable {
				func(iArgs...);
				});

			{
				std::scoped_lock lock(m_Mutex);
				m_Tasks.emplace([task] { (*task)(); });
			}

			m_ThreadCV.notify_one();
		}




	private:

		void Run();
	
		SharedPtr<PMRResource> GetResource() const;

		Vector<std::jthread> m_Workers;
		uint32_t m_ThreadCount;
		std::queue<std::packaged_task<void()>> m_Tasks;
		std::condition_variable m_ThreadCV;
		std::mutex m_Mutex;
		bool m_Stop;
	};
}

