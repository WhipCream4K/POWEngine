#pragma once

#include "POWEngine/Core/CustomTypes.h"
#include "POWEngine/Core/Thread/SimpleThreadPool.h"
#include "POWEngine/LockFree/LFQueue.h"
//#include "POWEngine/Services/Service.h"
#include "POWEngine/Logger/Logger.h"

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

		// TODO: Rule of 5
		ConsoleLogger();
		ConsoleLogger(const ConsoleLogger&) = delete;
		ConsoleLogger& operator=(const ConsoleLogger&) = delete;
		ConsoleLogger(ConsoleLogger&&) = delete;
		ConsoleLogger& operator=(ConsoleLogger&&) = delete;
		~ConsoleLogger() override;

	public:

		void AddMessage(const std::string& msg);

		//void AddInfoMessage(const std::string& msg);
		//void AddWarningMessage(const std::string& msg,const std::string& where);
		//void AddErrorMessage(const std::string& msg,const std::string& where);

		void LogErrorMessage(const std::string&, const std::string& fromWhere = "", bool showOrigin = false) override;
		void LogInfoMessage(const std::string&, const std::string& fromWhere = "", bool showOrigin = false) override;
		void LogWarningMessage(const std::string&, const std::string& fromWhere = "", bool showOrigin = false) override;

	private:

		void Run();

		//static inline void DisplayConsoleMessage(const std::string& msg);

		LFQueue<std::string> m_MessageQueue;
		std::condition_variable m_TaskWait;
		std::mutex m_Mutex;
		std::future<void> m_MessageThread;
		bool m_IsContextExit;
	};
}




