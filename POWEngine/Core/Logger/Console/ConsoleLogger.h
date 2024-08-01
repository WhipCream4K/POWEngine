#pragma once

#include "Core/CustomTypes.h"
//#include "POWEngine/Core/Thread/SimpleThreadPool.h"
//#include "POWEngine/LockFree/LFQueue.h"
#include "Core/Logger/Logger.h"

namespace powe
{

#define LogInfo				"[LogInfo]:"
#define LogWarning			"[LogWarning]:"
#define LogError			"[LogError]:"
#define GreenText			"\033[38;2;100;255;94m"
#define YellowText			"\033[38;2;238;238;155m"
#define RedText				"\033[38;2;255;0;0m"
#define ResetText			"\033[0m"

	class ConsoleLogger : public Logger
	{

	public:


		ConsoleLogger();
		ConsoleLogger(const ConsoleLogger&) = delete;
		ConsoleLogger& operator=(const ConsoleLogger&) = delete;
		ConsoleLogger(ConsoleLogger&&) = delete;
		ConsoleLogger& operator=(ConsoleLogger&&) = delete;
		~ConsoleLogger() override;

	public:

		void AddMessage(const std::string& msg);
		void Log(LogSeverity severity, const std::string& message, const std::string& fromWhere = "") override;
	};
}




