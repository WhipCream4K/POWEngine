#pragma once

namespace powe
{
	/**
	 * \brief In order to inherit from singleton class make sure that your class is thread safe
	 * and use the factory create to initialize singleton in the main thread to get rid of
	 * thread safe singleton initialization
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

	//template <typename T>
	//Singleton<T>::Singleton(Singleton&& other) noexcept
	//{
	//	m_Instance = std::move(other.m_Instance);
	//}

	//template <typename T>
	//Singleton<T>& Singleton<T>::operator=(Singleton&& other) noexcept
	//{
	//	if(*this != other)
	//	{
	//		m_Instance = std::move(other.m_Instance);
	//	}

	//	return this;
	//}

	template<typename T>
	SharedPtr<T> Singleton<T>::m_Instance{};
}


