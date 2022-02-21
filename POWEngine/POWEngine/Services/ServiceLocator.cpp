#include "pch.h"
#include "ServiceLocator.h"
#include "POWEngine/Logger/Logger.h"

void powe::ServiceLocator::Initialize()
{
	// Initialize logger
	{
		const auto serviceLocator{ GetInstance() };
		const auto logger{ std::make_shared<Logger>() };
		const size_t serviceId{ Service::GetId<Logger>() };
		serviceLocator->m_ServiceTable[serviceId] = logger;

		logger->AddMessage("Hello from logger");
	}
}
