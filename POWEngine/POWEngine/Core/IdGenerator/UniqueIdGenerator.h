#pragma once

namespace powe
{
	/**
	 * \brief Create Id per class basis
	 * \tparam T Class Type
	 */
	template<typename T>
	class UniqueIdGenerator
	{
	public:

		template<typename U>
		static size_t GetNewId();

	private:

		static size_t m_Count;
	};

	template <typename T>
	template <typename U>
	size_t UniqueIdGenerator<T>::GetNewId()
	{
		static const size_t counter{ m_Count++ };
		return counter;
	}

	template<typename T>
	size_t UniqueIdGenerator<T>::m_Count{ 1 };
}


