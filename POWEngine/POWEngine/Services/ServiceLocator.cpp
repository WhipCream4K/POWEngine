#include "pch.h"
#include "ServiceLocator.h"
#include "POWEngine/Logger/LoggerUtils.h"
#include "POWEngine/Sound/SoundSystem.h"

powe::NullSoundSystem powe::ServiceLocator::m_NullAudio{};
SharedPtr<powe::SoundSystem> powe::ServiceLocator::m_SoundSystem{ SharedPtr<powe::NullSoundSystem>(&ServiceLocator::m_NullAudio,[](powe::NullSoundSystem*){})};

powe::NullLogger powe::ServiceLocator::m_NullLogger{};
SharedPtr<powe::Logger> powe::ServiceLocator::m_Logger{ SharedPtr<powe::NullLogger>(&powe::ServiceLocator::m_NullLogger,[](powe::NullLogger*) {}) };


//void powe::ServiceLocator::Initialize()
//{
//	// Initialize logger
//	{
//		const auto serviceLocator{ GetInstance() };
//		const auto logger{ std::make_shared<Logger>() };
//		const size_t serviceId{ Service::GetId<Logger>() };
//		serviceLocator->m_ServiceTable.try_emplace(serviceId, logger);
//
//		POWLOGINFO("Message Thread Started");
//		POWLOGERROR("LogError test");
//		POWLOGWARNING("LogWarning Test");
//	}
//}

void powe::ServiceLocator::RegisterSoundSystem(const SharedPtr<SoundSystem>& soundSystem)
{
	m_SoundSystem = soundSystem == nullptr ? SharedPtr<NullSoundSystem>(&m_NullAudio,[](NullSoundSystem*){}) : soundSystem;

	std::string testMsg{ "Registering Audio system typeof -> " };
	testMsg.append(typeid(*soundSystem).name());
	POWLOGNORMAL(testMsg);
}

void powe::ServiceLocator::RegisterLogger(const SharedPtr<Logger>& logger)
{
	m_Logger = logger == nullptr ? SharedPtr<NullLogger>(&m_NullLogger, [](NullLogger*) {}) : logger;

	std::string testMsg{ "Registering Logger system typeof -> " };
	testMsg.append(typeid(*logger).name());
	POWLOGNORMAL(testMsg);
}
