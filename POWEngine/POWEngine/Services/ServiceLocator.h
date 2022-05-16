#pragma once

#include "Service.h"
#include "POWEngine/Sound/NullSoundSystem.h"
#include "POWEngine/Logger/NullLogger.h"

namespace powe
{
	class Service;
	class SoundSystem;
	class ConsoleLogger;
	class ServiceLocator final
	{

	public:

		//static void Initialize();

		//template<typename ServiceType>
		//static EnableIsBasedOf<Service,ServiceType,ServiceType&> GetService();

		//template<typename ServiceType,typename = EnableIsBasedOf<Service,ServiceType>>
		//static void RegisterService(const SharedPtr<Service>& service);

		ServiceLocator() = delete;
		ServiceLocator(const ServiceLocator&) = delete;
		ServiceLocator& operator=(const ServiceLocator&) = delete;
		ServiceLocator(ServiceLocator&&) noexcept = delete;
		ServiceLocator& operator=(ServiceLocator&&) noexcept = delete;
		~ServiceLocator() = default;

		static SoundSystem& GetSoundSystem() { return *m_SoundSystem; }
		static void RegisterSoundSystem(const SharedPtr<SoundSystem>& soundSystem);

		static Logger& GetLogger() { return *m_Logger; }
		static void RegisterLogger(const SharedPtr<Logger>& logger);

	private:

		// ----- Audio ------
		static SharedPtr<SoundSystem> m_SoundSystem;
		static NullSoundSystem m_NullAudio;
		// ------------------

		// ----- Logger ------
		static SharedPtr<Logger> m_Logger;
		static NullLogger m_NullLogger;
		// -------------------
	};
}


