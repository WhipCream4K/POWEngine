#pragma once

#include "Service.h"
#include "POWEngine/Singleton/Singleton.h"
#include "POWEngine/Core/IdGenerator/UniqueIdGenerator.h"

namespace powe
{
	class Service;
	class ServiceLocator final : public Singleton<ServiceLocator>
	{

		friend class Singleton;

		using ServiceKey = size_t;

	public:

		static void Initialize();

		template<typename ServiceType>
		static SharedPtr<ServiceType> GetService();

		ServiceLocator(const ServiceLocator&) = delete;
		ServiceLocator& operator=(const ServiceLocator&) = delete;
		ServiceLocator(ServiceLocator&&) = default;
		ServiceLocator& operator=(ServiceLocator&&) = default;
		~ServiceLocator() override = default;


	private:

		std::unordered_map<ServiceKey, SharedPtr<Service>> m_ServiceTable;
	};

	template <typename ServiceType>
	SharedPtr<ServiceType> ServiceLocator::GetService()
	{
		const size_t serviceId{ Service::GetId<ServiceType>() };
		const auto serviceLocator{ GetInstance() };

		const auto itr{ serviceLocator->m_ServiceTable.find(serviceId) };
		if (itr != serviceLocator->m_ServiceTable.end())
			return itr->second;

		return {};
	}
}


