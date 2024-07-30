#pragma once

#include "Core/CustomTypes.h"

namespace powe
{
	template<typename T>
	class ThreadSafeSingleton
	{
	public:

		// Using Double-Checked Locking Pattern
		static T& Instance()
		{
			if (m_Instance == nullptr)
			{
				std::scoped_lock lock(m_Mutex);
				if (m_Instance == nullptr)
				{
					m_Instance = std::make_unique<T>();
				}
			}

			return *m_Instance.get();
		}


		ThreadSafeSingleton(const ThreadSafeSingleton&) = delete;
		ThreadSafeSingleton& operator=(const ThreadSafeSingleton&) = delete;
		ThreadSafeSingleton(ThreadSafeSingleton&&) = delete;
		ThreadSafeSingleton& operator=(ThreadSafeSingleton&&) = delete;
		virtual ~ThreadSafeSingleton() = default;

	protected:

		ThreadSafeSingleton() = default;

		static UniquePtr<T> m_Instance; // since c++20
		static std::mutex m_Mutex;
	};

	template<typename T>
	UniquePtr<T> ThreadSafeSingleton<T>::m_Instance{};

}


