#include "pch.h"
#include "ConsoleLogger.h"
#include "Core/Application/Application.h"
#include "Utils/Utils.h"
#include "Logger/Logger.h"

#include <iostream>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define WHITE_TEXT_BLACK_BG FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY

#endif

powe::ConsoleLogger::ConsoleLogger()
	: m_Stop(false)
{
	// Because LFQueue is not copy constructable
	new (&m_MessageQueue) LFQueue<LogMsg>(GetUpStream().get());
	
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

void powe::ConsoleLogger::LogLevel(LogSeverity severity, const std::function<std::string()>& format)
{
	m_MessageQueue.push(LogMsg(severity, format));
	m_ThreadCV.notify_one();
}

void powe::ConsoleLogger::Run()
{
	while (true)
	{
		LogMsg msg{};

		{
			std::unique_lock lock(m_Mutex);
			m_ThreadCV.wait(lock, [this] {return m_Stop || !m_MessageQueue.empty();});

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

		const std::string fullmsg{ std::string(LogInfo )+ " " + msg.format() };

		std::clog << fullmsg << '\n';

#ifdef _WIN32

		SetConsoleTextAttribute(consoleHandle, WHITE_TEXT_BLACK_BG);

#endif
	}
}

powe::SharedPtr<powe::PMRResource> powe::ConsoleLogger::GetUpStream() const noexcept
{
	if(const auto logger{ Application::GetModule<powe::Logger>() })
	{
		return logger->GetResource();
	}
	return GetAppResource();
}