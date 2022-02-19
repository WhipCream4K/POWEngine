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

powe::Logger::~Logger()
{
	m_MessageQueue.Push("Message thread exit");
	m_TaskWait.notify_all();

	m_IsContextExit = true;
}

void powe::Logger::Run()
{
	// this is single producer so we don't need atomic bool check
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


}

void powe::Logger::DisplayConsoleMessage(const std::string& msg)
{
	std::cout << msg << '\n';
}

//void powe::Logger::DisplayConsoleMessage(const std::string& msg) const
//{
//	std::cout << msg << '\n';
//}
