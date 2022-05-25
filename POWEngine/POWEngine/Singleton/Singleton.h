#pragma once
#include "POWEngine/Core/CustomTypes.h"
#include "InstanceUtils.h"

namespace powe
{
	/**
	 * \brief Non thread-safe singleton
	 * \tparam T Class Type
	 */
	template<typename T>
	class Singleton
	{
	public:

		static SharedPtr<T> GetInstance()
		{
			if (!m_Instance)
				m_Instance = SharedPtr<T>(new T());
			return m_Instance;
		}

		Singleton(const Singleton&) = delete;
		Singleton& operator=(const Singleton&) = delete;
		Singleton(Singleton&& other) noexcept = delete;
		Singleton& operator=(Singleton&& other) noexcept = delete;

		virtual ~Singleton() = default;

	protected:

		Singleton() = default;

		static SharedPtr<T> m_Instance;
	};

	template<typename T>
	SharedPtr<T> Singleton<T>::m_Instance{};
}


