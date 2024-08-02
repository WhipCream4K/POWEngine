
#pragma once

#include "Core/CustomTypes.h"
#include <thread>
#include <future>
#include <queue>
#include <functional>

#include "Core/Application/Application.h"

namespace powe
{
	class SimpleThreadPool
	{
	public:

		SimpleThreadPool(std::pmr::memory_resource* memResource, size_t threadCount = (size_t)std::thread::hardware_concurrency());
		SimpleThreadPool(const SimpleThreadPool&) = delete;
		SimpleThreadPool& operator=(const SimpleThreadPool&) = delete;
		SimpleThreadPool(SimpleThreadPool&&) noexcept = delete;
		SimpleThreadPool& operator=(SimpleThreadPool&&) noexcept = delete;
		~SimpleThreadPool();

	public:

		template<typename Func, typename ... Args, typename Ret = std::invoke_result_t<Func, Args...>>
		constexpr std::future<Ret> EnqueueFuture(Func&& fn, Args&&... args)
		{

			std::pmr::polymorphic_allocator<std::packaged_task<Ret(Args...)>> allocator{ m_MemResource };

			auto task = std::allocate_shared<std::packaged_task<Ret(Args...)>>(allocator, [func = std::forward<Func>(fn), ...args = std::forward<Args>(args)]() mutable {
				return func(std::forward<Args>(args)...);
				});

			std::future<Ret> res = task->get_future();

			{
				std::scoped_lock lock(m_Mutex);

				// Don't allow enqueueing after stopping the pool
				if (m_Stop)
					throw std::runtime_error("enqueue on stopped ThreadPool");

				m_Tasks.emplace([task] { (*task)(); });
			}

			m_ThreadCV.notify_one();

			return res;
		}

		template<typename Func, typename ... Args>
		constexpr void Enqueue(Func&& fn, Args&&... args)
		{
			std::pmr::polymorphic_allocator<std::packaged_task<void()>> allocator{ m_MemResource };

			auto task = std::allocate_shared<std::packaged_task<void()>>(allocator, [func = std::forward<Func>(fn), ...args = std::forward<Args>(args)]() mutable {
				func(std::forward<Args>(args)...);
				});

			{
				std::scoped_lock lock(m_Mutex);

				// Don't allow enqueueing after stopping the pool
				if (m_Stop)
					throw std::runtime_error("enqueue on stopped ThreadPool");

				m_Tasks.emplace([task] { (*task)(); });
			}

			m_ThreadCV.notify_one();
		}




	private:

		void Run();

		std::pmr::memory_resource* m_MemResource;
		Vector<std::jthread> m_Workers;
		std::queue<std::packaged_task<void()>> m_Tasks;
		std::condition_variable m_ThreadCV;
		std::mutex m_Mutex;
		bool m_Stop;
	};


	SimpleThreadPool& GetThreadPool()
	{
		return Application::Get().GetThreadPool();
	}
}

