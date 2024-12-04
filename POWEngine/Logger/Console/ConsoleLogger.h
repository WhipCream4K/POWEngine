#pragma once

#include "Core/CustomTypes.h"
#include "Logger/BaseLogger.h"
#include "LockFree/LFQueue.h"

namespace powe
{

#define LogInfo				"[LogInfo]:"
#define LogWarning			"[LogWarning]:"
#define LogError			"[LogError]:"
#define GreenText			"\033[38;2;100;255;94m"
#define YellowText			"\033[38;2;238;238;155m"
#define RedText				"\033[38;2;255;0;0m"
#define ResetText			"\033[0m"

	class ConsoleLogger : public BaseLogger
	{

		struct LogMsg
		{
			LogSeverity severity;
			std::function<std::string()> format;
		};

	public:


		ConsoleLogger();
		ConsoleLogger(const ConsoleLogger&) = delete;
		ConsoleLogger& operator=(const ConsoleLogger&) = delete;
		ConsoleLogger(ConsoleLogger&&) noexcept = delete;
		ConsoleLogger& operator=(ConsoleLogger&&) noexcept = delete;
		~ConsoleLogger() override;

		void LogLevel(LogSeverity severity, const std::function<std::string()>& format) override;

	private:

		void Run();
		SharedPtr<PMRResource> GetUpStream() const noexcept;

		LFQueue<LogMsg> m_MessageQueue;
		std::jthread m_MessageThread;
		std::mutex m_Mutex;
		std::condition_variable m_ThreadCV;

		bool m_Stop;

	};
}




