#pragma once

#include "POWEngine/Singleton/Singleton.h"
#include "POWEngine/Core/IdGenerator/UniqueIdGenerator.h"

namespace powe
{
	// Thread safe service locator
	class ServiceLocator final : public Singleton<ServiceLocator>
	{

		friend class Singleton;

	public:

		static void Initialize();

		template<typename ServiceType>
		static const SharedPtr<ServiceType>& GetService();

	private:


	};

	template <typename ServiceType>
	const SharedPtr<ServiceType>& ServiceLocator::GetService()
	{
		//const size_t serviceId{ UniqueIdGenerator<ServiceType>::GetNewId() };
	}
}


