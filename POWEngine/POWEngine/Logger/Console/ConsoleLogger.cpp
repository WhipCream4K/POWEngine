#include "pch.h"
#include "ConsoleLogger.h"
#include <iostream>

powe::ConsoleLogger::ConsoleLogger()
	: m_MessageQueue()
	, m_TaskWait()
	, m_Mutex()
	, m_MessageThread()
	, m_IsContextExit()
{
	m_MessageThread = std::async(std::launch::async, &ConsoleLogger::Run, this);
}

void powe::ConsoleLogger::AddMessage(const std::string& msg)
{
	m_MessageQueue.Push(msg);
	m_TaskWait.notify_one();
}

//void powe::ConsoleLogger::AddInfoMessage(const std::string& msg)
//{
//	std::string log{ GreenText };
//	log.append(LogInfo ResetText " " + msg);
//	AddMessage(log);
//}
//
//void powe::ConsoleLogger::AddWarningMessage(const std::string& msg, const std::string& where)
//{
//	std::string log{ YellowText };
//	log.append(LogWarning ResetText " " + msg + " " + where);
//	AddMessage(log);
//}
//
//void powe::ConsoleLogger::AddErrorMessage(const std::string& msg, const std::string& where)
//{
//	std::string log{ RedText };
//	log.append(LogError ResetText " " + msg + " " + where);
//	AddMessage(log);	
//}

void powe::ConsoleLogger::LogErrorMessage(const std::string& msg, const std::string& fromWhere)
{
	std::string log{ RedText };
	log.append(LogError ResetText " " + msg + " " + fromWhere);
	AddMessage(log);
}

void powe::ConsoleLogger::LogInfoMessage(const std::string& msg, const std::string& fromWhere)
{
	std::string log{ GreenText };
	log.append(LogInfo ResetText " " + msg + " " + fromWhere);
	AddMessage(log);
}

void powe::ConsoleLogger::LogWarningMessage(const std::string& msg, const std::string& fromWhere)
{
	std::string log{ YellowText };
	log.append(LogWarning ResetText " " + msg + " " + fromWhere);
	AddMessage(log);
}

powe::ConsoleLogger::~ConsoleLogger()
{
	// notify the thread so we can exit
	//LogInfoMessage("Message Thread Exiting");
	const std::string exit{ "Message Thread Exiting" };
	const std::string loggerExitMsg{ LogInfo ResetText " " + exit };
	AddMessage(loggerExitMsg);
	m_IsContextExit = true;
}

void powe::ConsoleLogger::Run()
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
			std::cout << msg.value() << '\n';
	}

	std::cout << GreenText LogInfo ResetText " Message Thread exit successfully\n";
}

//void powe::ConsoleLogger::DisplayConsoleMessage(const std::string& msg)
//{
//	std::cout << msg << '\n';
//}

//void powe::Logger::DisplayConsoleMessage(const std::string& msg) const
//{
//	std::cout << msg << '\n';
//}
