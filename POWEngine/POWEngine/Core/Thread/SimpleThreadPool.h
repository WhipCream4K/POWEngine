
#pragma once

#include "POWEngine/Core/CustomTypes.h"
#include <thread>
#include <future>
#include <queue>
#include <functional>

namespace powe
{
	class SimpleThreadPool
	{
	public:

		SimpleThreadPool(size_t threadCount = (size_t)std::thread::hardware_concurrency());
		SimpleThreadPool(const SimpleThreadPool&) = delete;
		SimpleThreadPool& operator=(const SimpleThreadPool&) = delete;
		SimpleThreadPool(SimpleThreadPool&&) noexcept = delete;
		SimpleThreadPool& operator=(SimpleThreadPool&&) noexcept = delete;
		~SimpleThreadPool();

	public:

		// TODO: MARK maybe this should be depricate because object in memory can be moved everywhere and it's going to blow up
		// template<
		// 	typename Func,
		// 	typename UserClass = typename FnTraits<Func>::class_type,
		// 	typename ...Args,
		// 	typename Ret = std::invoke_result_t<Func, UserClass, Args...>>
		// 	std::future<Ret> PushMemberTask(Func&& fn, UserClass* obj, Args&&... args);

		template<
			typename Func,
		typename UserClass,
		typename ...Args,
		typename Ret = std::invoke_result_t<Func,UserClass,Args...>>
		std::future<Ret> PushMemberTask(Func&& fn,UserClass obj,Args&&... args);

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
	
	// template <typename Func, typename UserClass, typename ... Args, typename Ret>
	// std::future<Ret> SimpleThreadPool::PushMemberTask(Func&& fn, UserClass* obj, Args&&... args)
	// {
	// 	std::packaged_task<void()> task{};
	// 	std::future<Ret> future{};
	//
	// 	if constexpr (std::is_void_v<Ret>)
	// 	{
	// 		task = std::packaged_task<void()>{ [
	// 			func = std::forward<Func>(fn),
	// 		&obj,
	// 		... largs = std::forward<Args>(args)] ()
	// 		{
	// 			(obj->*func)(largs...);
	// 		} };
	//
	// 		future = task.get_future();
	// 	}
	// 	else
	// 	{
	// 		auto sharedPromise{ std::make_shared<std::promise<Ret>>() };
	//
	// 		// Perfect capture c++20 https://stackoverflow.com/questions/47496358/c-lambdas-how-to-capture-variadic-parameter-pack-from-the-upper-scope
	// 		task = std::packaged_task<void()>{ [
	// 			func = std::forward<Func>(fn),
	// 				&obj,
	// 				... largs = std::forward<Args>(args) ,
	// 				sharedPromise] ()
	// 			{
	// 				sharedPromise->set_value(std::invoke(func,obj,largs...));
	// 			} };
	//
	//
	// 		future = sharedPromise->get_future();
	// 	}
	//
	//
	// 	{
	// 		std::unique_lock lock{ m_Mutex };
	// 		m_Tasks.emplace(std::move(task));
	// 	}
	//
	//
	// 	m_TaskWait.notify_one();
	//
	// 	return future;
	// }

	template <typename Func, typename UserClass, typename ... Args, typename Ret>
	std::future<Ret> SimpleThreadPool::PushMemberTask(Func&& fn, UserClass obj, Args&&... args)
	{
		std::packaged_task<void()> task{};
		std::future<Ret> future{};
		WeakPtr<typename UserClass::element_type> weakCaller{obj};

		if constexpr (std::is_void_v<Ret>)
		{
			task = std::packaged_task<void()>{ [
				weakCaller,
				func = std::forward<Func>(fn),
			... largs = std::forward<Args>(args)] () mutable 
			{
				if(auto caller = weakCaller.lock(); caller)
					(*caller.*func)(largs...);
					// std::invoke(func,caller,largs...);
			} };

			future = task.get_future();
		}
		else
		{
			auto sharedPromise{ std::make_shared<std::promise<Ret>>() };

			// Perfect capture c++20 https://stackoverflow.com/questions/47496358/c-lambdas-how-to-capture-variadic-parameter-pack-from-the-upper-scope
			task = std::packaged_task<void()>{ [
				weakCaller,
				func = std::forward<Func>(fn),
					... largs = std::forward<Args>(args) ,
					sharedPromise] ()
			{
				if(auto  caller = weakCaller.lock(); caller)
					sharedPromise->set_value(std::invoke(func,caller,largs...));
			} };
			
			future = sharedPromise->get_future();
		}


		{
			std::scoped_lock lock{ m_Mutex };
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
		std::future<Ret> future{ sharedPromise->get_future() };
		std::packaged_task<void()> task{};

		// Perfect capture
		task = std::move(std::packaged_task<void()>{ [
			func = std::forward<Func>(fn),
				... largs = std::forward<Args>(args),
				sharedPromise] ()
			{
				sharedPromise->set_value(func(largs...));
			}});

		{
			std::scoped_lock lock{ m_Mutex };
			m_Tasks.emplace(std::move(task));
		}

		m_TaskWait.notify_one();

		return future;
	}
}

