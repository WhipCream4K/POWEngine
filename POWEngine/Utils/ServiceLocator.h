#pragma once

#include "Service.h"
#include "Core/Logger/Logger.h"
#include "Sound/SoundService.h"

namespace powe
{
	class IService;
	class SoundService;
	class ConsoleLogger;
	class CoreResource;
	class ICore;
	class ServiceLocator final
	{

	public:

		ServiceLocator() = delete;
		ServiceLocator(const ServiceLocator&) = delete;
		ServiceLocator& operator=(const ServiceLocator&) = delete;
		ServiceLocator(ServiceLocator&&) noexcept = delete;
		ServiceLocator& operator=(ServiceLocator&&) noexcept = delete;
		~ServiceLocator() = default;

		static SoundService& GetSoundSystem() { return *m_SoundSystem; }
		static void RegisterSoundSystem(const SharedPtr<SoundService>& soundSystem);

		static Logger& GetLogger() { return *m_Logger; }
		static void RegisterLogger(const SharedPtr<Logger>& logger);


		// Not thread-safe method to register a service
		template<typename T>
		void RegisterService(const SharedPtr<T>& service)
		{
			static_assert(std::is_base_of_v<IService, T>, "Service must derive from IService");
			const std::string serviceType{ typeid(T).name() }; // There can only be one service of each type
			m_Services.try_emplace(serviceType, service);
		}

		template<typename T>
		WeakPtr<T> GetService()
		{
			static_assert(std::is_base_of_v<IService, T>, "Service must derive from IService");
			const std::string serviceType{ typeid(T).name() };
			const auto it{ m_Services.find(serviceType) };
			if (it != m_Services.end())
			{
				return it->second.get();
			}
			return nullptr;
		}
		
	private:

		std::unordered_map<std::string, SharedPtr<IService>> m_Services;

		// ----- Audio ------
		static SharedPtr<SoundService> m_SoundSystem;
		// ------------------

		// ----- Logger ------
		static SharedPtr<Logger> m_Logger;
		// -------------------

		// ----- Engine Core ------
		//static SharedPtr<ICore> m_CoreInterface;
	};
}


