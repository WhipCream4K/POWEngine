#pragma once

#include "Service.h"
#include "POWEngine/Singleton/Singleton.h"

namespace powe
{
	class Service;
	class ServiceLocator final : public Singleton<ServiceLocator>
	{

		friend class Singleton<ServiceLocator>;
		using ServiceKey = size_t;

	public:

		ServiceLocator() = default;

		static void Initialize();

		template<typename ServiceType>
		static SharedPtr<EnableIsBasedOf<Service,ServiceType,ServiceType>> GetService();

		ServiceLocator(const ServiceLocator&) = delete;
		ServiceLocator& operator=(const ServiceLocator&) = delete;
		ServiceLocator(ServiceLocator&&) noexcept = default;
		ServiceLocator& operator=(ServiceLocator&&) noexcept = default;
		~ServiceLocator() override = default;


	private:

		std::unordered_map<ServiceKey, SharedPtr<Service>> m_ServiceTable;
	};

	template <typename ServiceType>
	SharedPtr<EnableIsBasedOf<Service, ServiceType, ServiceType>> ServiceLocator::GetService()
	{
		const size_t serviceId{ Service::GetId<ServiceType>() };
		const auto serviceLocator{ GetInstance() };

		const auto itr{ serviceLocator->m_ServiceTable.find(serviceId) };
		if (itr != serviceLocator->m_ServiceTable.end())
			return std::static_pointer_cast<ServiceType>(itr->second);

		return {};
	}
}


