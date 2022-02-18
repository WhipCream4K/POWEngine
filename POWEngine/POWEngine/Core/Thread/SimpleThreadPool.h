
#pragma once

#include "POWEngine/Core/CustomTypes.h"
#include <thread>
#include <future>
#include <queue>

namespace powe
{
	class SimpleThreadPool
	{
	public:

		SimpleThreadPool(size_t threadCount = (size_t)std::thread::hardware_concurrency());
		~SimpleThreadPool();

		// TODO: MARK maybe this should be depricate because object in memory can be moved everywhere and it's going to blow up
		template<
			typename Func,
			typename UserClass = typename FnTraits<Func>::class_type,
			typename ...Args,
			typename Ret = std::invoke_result_t<Func, UserClass, Args...>>
			std::future<Ret> PushStaticObjectTask(Func&& fn, UserClass& obj, Args&&... args);

		template<
			typename Func,
			typename ...Args,
			typename Ret = std::invoke_result_t<Func, Args...>>
			std::future<Ret> PushTask(Func&& fn, Args&&... args);


	private:

		void Run();

		std::vector<std::future<void>> m_Workers;
		std::queue<std::packaged_task<void()>> m_Tasks;
		std::condition_variable m_TaskWait;
		std::mutex m_Mutex;
	};


	template <typename Func, typename UserClass, typename ... Args, typename Ret>
	std::future<Ret> SimpleThreadPool::PushStaticObjectTask(Func&& fn, UserClass& obj, Args&&... args)
	{
		std::packaged_task<void()> task{};

		auto sharedPromise{ std::make_shared<std::promise<Ret>>() };

		// Perfect capture c++20 https://stackoverflow.com/questions/47496358/c-lambdas-how-to-capture-variadic-parameter-pack-from-the-upper-scope
		task = std::packaged_task<void()>{ [
			func = std::forward<Func>(fn),
				&obj,
				... largs = std::forward<Args>(args) ,
				sharedPromise] ()
			{
				sharedPromise->set_value(std::invoke(func,obj,largs...));
			} };


		std::future<Ret> future{ sharedPromise->get_future() };

		{
			std::unique_lock lock{ m_Mutex };
			m_Tasks.emplace(std::move(task));
		}


		m_TaskWait.notify_one();

		return future;
	}

	template <typename Func, typename ... Args, typename Ret>
	std::future<Ret> SimpleThreadPool::PushTask(Func&& fn, Args&&... args)
	{
		// if I don't create promise with shared pointer then when I retrieved future object it's referencing null object
		auto sharedPromise{ std::make_shared<std::promise<Ret>>() };
		std::packaged_task<void()> task{};

		// Perfect capture
		task = std::move(std::packaged_task<void()>{ [
			func = std::forward<Func>(fn),
				... largs = std::forward<Args>(args),
				sharedPromise] ()
			{
				sharedPromise->set_value(func(largs...));
			}});

		std::future<Ret> future{ sharedPromise->get_future() };

		{
			std::unique_lock lock{ m_Mutex };
			m_Tasks.emplace(std::move(task));
		}

		m_TaskWait.notify_one();

		return future;
	}
}

