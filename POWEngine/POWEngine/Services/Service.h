#pragma once

namespace powe
{
	/**
	 * \brief Just a base class for all Service types
	 */
	class Service
	{
	public:

		Service() = default;
		Service(const Service&) = delete;
		Service& operator=(const Service&) = delete;
		Service(Service&&) = default;
		Service& operator=(Service&&) = default;
		virtual ~Service() = default;

		// Generate Id for their sub class
		template<typename T>
		static size_t GetId();

	private:

		// Thread safe
		static std::atomic_size_t m_Id;

	};

	template <typename T>
	size_t Service::GetId()
	{
		static const size_t counter{ m_Id++ };
		return counter;
	}


}



