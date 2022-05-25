#pragma once

#include "POWEngine/Core/CustomTypes.h"
#include "InstanceUtils.h"
#include <mutex>

namespace powe
{
	template<typename T>
	class ThreadSafeSingleton
	{
	public:

		static SharedPtr<T> GetInstance()
		{
			SharedPtr<T> old{ m_Instance.load() };

			if (old)
				return old;

			SharedPtr<T> null{};
			m_Instance.compare_exchange_weak(null, SharedPtr<T>(new T()));
			return m_Instance.load();
		}


		ThreadSafeSingleton(const ThreadSafeSingleton&) = delete;
		ThreadSafeSingleton& operator=(const ThreadSafeSingleton&) = delete;
		ThreadSafeSingleton(ThreadSafeSingleton&&) = delete;
		ThreadSafeSingleton& operator=(ThreadSafeSingleton&&) = delete;
		virtual ~ThreadSafeSingleton() = default;

	protected:

		ThreadSafeSingleton() = default;

		static std::atomic<SharedPtr<T>> m_Instance; // since c++20
	};

	template<typename T>
	std::atomic<SharedPtr<T>> ThreadSafeSingleton<T>::m_Instance{};

}


