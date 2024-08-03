#pragma once

#include <variant>
#include <typeindex>

#include "Core/CustomTypes.h"
#include "Service.h"


namespace powe
{
	template<typename T>
	concept ServiceConcept = std::is_base_of_v<IService<T>, T>;

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

		template<typename T,typename... Args> requires is_one_of<T,IService...>
		void RegisterService(Args&&... args)
		{
			std::pmr::polymorphic_allocator<T> alloc{m_MemResource};
			SharedPtr<T> service{std::allocate_shared<T>(alloc,std::forward<Args>(args)...)};
			m_Services[std::type_index(typeid(T))] = service;
		}

		template<typename T> requires is_one_of<T,IService...>
		T* GetService()
		{
			const auto it{ m_Services.find(std::type_index(typeid(T))) };
			if (it != m_Services.end())
			{
				return std::get<SharedPtr<T>>(it->second).get();
			}
			return nullptr;
		}
		

	private:

        using ServiceVariant = std::variant<SharedPtr<IService>...>;

		UnOrderedMap<std::type_index, ServiceVariant> m_Services;
		std::pmr::memory_resource* m_MemResource;
	};
}


