#include "pch.h"
#include "ConsoleLogger.h"
#include "Core/Application/Application.h"


#include <iostream>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define WHITE_TEXT_BLACK_BG FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY

#endif

powe::ConsoleLogger::ConsoleLogger()
	: m_MessageQueue(GetResource())
	, m_Stop(false)
{
	m_MessageThread = std::jthread(&ConsoleLogger::Run, this);
}

powe::ConsoleLogger::~ConsoleLogger()
{
	{
		std::scoped_lock lock(m_Mutex);
		m_Stop = true;
	}

	m_ThreadCV.notify_all();
	m_MessageThread.join();
}

void powe::ConsoleLogger::LogLevel(LogSeverity severity, const std::string& message, const std::string& fromWhere)
{
	std::string log{};

	switch (severity)
	{
	case LogSeverity::Info:
		log.append(LogInfo + message + " " + fromWhere);
		break;
	case LogSeverity::Warning:
		log.append(LogWarning + message + " " + fromWhere);
		break;
	case LogSeverity::Error:
		log.append(LogError + message + " " + fromWhere);
		break;
	default:
		break;
	}

	m_MessageQueue.push(LogMsg(severity, log));
	m_ThreadCV.notify_one();
}

void powe::ConsoleLogger::Log(const std::string& message)
{
	m_MessageQueue.push(LogMsg(LogSeverity::Info, message));
	m_ThreadCV.notify_one();
}

void powe::ConsoleLogger::Run()
{
	while (true)
	{
		LogMsg msg;

		{
			std::unique_lock lock(m_Mutex);
			m_ThreadCV.wait(lock, [this] {return m_Stop || !m_MessageQueue.empty(); });

			if (m_Stop && m_MessageQueue.empty())
				return;
		}

		m_MessageQueue.pop(msg);

#ifdef _WIN32

		const HANDLE consoleHandle{ GetStdHandle(STD_OUTPUT_HANDLE) };

		switch (msg.severity)
		{
		case LogSeverity::Info:
			SetConsoleTextAttribute(consoleHandle, 15); // White text
			break;
		case LogSeverity::Warning:
			SetConsoleTextAttribute(consoleHandle, 14); // Yellow text
			break;
		case LogSeverity::Error:
			SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED);
			break;
		default:
			break;
		}

#endif

		std::cout << msg.message << '\n';

#ifdef _WIN32

		SetConsoleTextAttribute(consoleHandle, WHITE_TEXT_BLACK_BG);

#endif
	}
}

powe::PMRResource* powe::ConsoleLogger::GetResource() const
{
	MemoryManager* memManager{ Application::GetAppService<MemoryManager>() };
	if(memManager)
	{
		return memManager->GetAllocator("Application");
	}

	return std::pmr::get_default_resource();
}