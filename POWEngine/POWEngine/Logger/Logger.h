#pragma once

#include "POWEngine/Core/CustomTypes.h"
#include "POWEngine/Core/Thread/SimpleThreadPool.h"
#include "POWEngine/Core/LockFree/LFQueue.h"
#include "POWEngine/Services/Service.h"

namespace powe
{

#define LogInfo				"[LogInfo]:"
#define LogWarning			"[LogWarning]:"
#define LogError			"[LogError]:"
#define GreenText			"\033[38;2;100;255;94m"
#define YellowText			"\033[38;2;238;238;155m"
#define RedText				"\033[38;2;255;0;0m"
#define ResetText			"\033[0m"

	class Logger : public Service
	{

	public:

		// TODO: Rule of 5
		Logger();
		void AddMessage(const std::string& msg);
		void AddInfoMessage(const std::string& msg);
		void AddWarningMessage(const std::string& msg,const std::string& where);
		void AddErrorMessage(const std::string& msg,const std::string& where);
		~Logger() override;

	private:

		void Run();

		static inline void DisplayConsoleMessage(const std::string& msg);

		LFQueue<std::string> m_MessageQueue;
		std::condition_variable m_TaskWait;
		std::mutex m_Mutex;
		std::future<void> m_MessageThread;
		bool m_IsContextExit;
	};
}




