#pragma once

#include "POWEngine/Core/CustomTypes.h"
#include "POWEngine/Core/Thread/SimpleThreadPool.h"
#include "POWEngine/Core/LockFree/LFQueue.h"

#define COLOR "\033"

namespace powe
{
	class Logger
	{
	public:

		Logger();
		void AddMessage(const std::string& msg);
		~Logger();

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


