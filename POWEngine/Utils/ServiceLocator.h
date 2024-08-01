#pragma once

#include <variant>

#include "Core/CustomTypes.h"
#include "Utils/Utils.h"

namespace powe
{
	template<typename T>
	concept ServiceConcept = requires(T t)
	{
		{ t.ServiceType() } -> std::convertible_to<std::string>;
	};

	template<ServiceConcept... IService>
	class ServiceLocator final
	{

	public:

		ServiceLocator(std::pmr::memory_resource* memResource)
			: m_Services(memResource)
			, m_MemResource(memResource)
		{	
		}

		ServiceLocator(const ServiceLocator&) = delete;
		ServiceLocator& operator=(const ServiceLocator&) = delete;
		ServiceLocator(ServiceLocator&&) noexcept = default;
		ServiceLocator& operator=(ServiceLocator&&) noexcept = default;
		~ServiceLocator() = default;

		template<typename T> requires  is_one_of<T,IService...>
		void RegisterService(T&& object)
		{
			auto service{AllocateUnique<T>(std::move(object), m_MemResource)};
			const std::string serviceType{service->ServiceType()};
			m_Services.try_emplace(serviceType,std::move(service));
		}

		template<typename T> requires is_one_of<T,IService...>
		T* GetService()
		{
			const std::string serviceType{ std::declval<T>().ServiceType() };
			const auto it{ m_Services.find(serviceType) };
			if (it != m_Services.end())
			{
				return std::get<UniquePtr<T>>(it->second).get();
			}
			return nullptr;
		}
		

	private:

		using ServiceVariant = std::variant<UniquePtr<IService>...>;

		UnOrderedMap<std::string, ServiceVariant> m_Services;
		std::pmr::memory_resource* m_MemResource;
	};
}


