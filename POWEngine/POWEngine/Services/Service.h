#pragma once

#include <functional>

namespace powe
{
	using ServiceKey = uint32_t;

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

		//virtual std::function<SharedPtr<Service>> GetDefault() = 0;
		//virtual SharedPtr<Service> GetDefault() const = 0;

	private:

		// Thread safe
		static std::atomic<ServiceKey> m_Id;

	};

	template <typename T>
	ServiceKey Service::GetId()
	{
		static const ServiceKey counter{ m_Id.load() };
		++m_Id;
		return counter;
	}


}



