#include "pch.h"
#include "ServiceLocator.h"
#include "POWEngine/Logger/LoggingService.h" 

void powe::ServiceLocator::Initialize()
{
	// Initialize logger
	{
		const auto serviceLocator{ GetInstance() };
		const auto logger{ std::make_shared<Logger>() };
		const size_t serviceId{ Service::GetId<Logger>() };
		serviceLocator->m_ServiceTable.try_emplace(serviceId, logger);

		POWLOGINFO("Message Thread Started");
		POWLOGERROR("LogError test");
		POWLOGWARNING("LogWarning Test");
	}
}
