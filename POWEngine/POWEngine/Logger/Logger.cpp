#include "pch.h"
#include "Logger.h"
#include <iostream>

powe::Logger::Logger()
	: m_MessageQueue()
	, m_TaskWait()
	, m_Mutex()
	, m_MessageThread()
	, m_IsContextExit()
{
	m_MessageThread = std::async(std::launch::async, &Logger::Run, this);
}

void powe::Logger::AddMessage(const std::string& msg)
{
	m_MessageQueue.Push(msg);
	m_TaskWait.notify_one();
}

void powe::Logger::AddInfoMessage(const std::string& msg)
{
	std::string log{ GreenText };
	log.append(LogInfo ResetText " " + msg);
	AddMessage(log);
}

void powe::Logger::AddWarningMessage(const std::string& msg)
{
	std::string log{ YellowText };
	log.append(LogWarning ResetText " " + msg);
	AddMessage(log);
}

void powe::Logger::AddErrorMessage(const std::string& msg)
{
	std::string log{ RedText };
	log.append(LogError ResetText " " + msg);
	AddMessage(log);
}

powe::Logger::~Logger()
{
	// notify the thread so we can exit
	AddInfoMessage("Message Thread Exiting");
	m_IsContextExit = true;
}

void powe::Logger::Run()
{
	// this is single a producer so we don't need atomic bool check or anything
	while (!m_IsContextExit)
	{
		std::optional<std::string> msg{};

		{
			std::unique_lock lock{ m_Mutex };

			m_TaskWait.wait(lock, [this]()
				{
					return !m_MessageQueue.Empty();
				});
		}

		msg = m_MessageQueue.PopReturn();

		if (msg.has_value())
			DisplayConsoleMessage(msg.value());
	}

	std::cout << GreenText LogInfo ResetText " Message Thread exit successfully\n";
}

void powe::Logger::DisplayConsoleMessage(const std::string& msg)
{
	std::cout << msg << '\n';
}

//void powe::Logger::DisplayConsoleMessage(const std::string& msg) const
//{
//	std::cout << msg << '\n';
//}
