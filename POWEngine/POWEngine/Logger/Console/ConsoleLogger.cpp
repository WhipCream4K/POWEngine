#include "pch.h"
#include "ConsoleLogger.h"
#include <iostream>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define WHITE_TEXT_BLACK_BG FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY


#endif

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

void powe::ConsoleLogger::LogErrorMessage(const std::string& msg, const std::string& fromWhere, bool)
{
#ifdef _WIN32

    const HANDLE consoleHandle{GetStdHandle(STD_OUTPUT_HANDLE)};
    SetConsoleTextAttribute(consoleHandle, FOREGROUND_RED);
    const std::string log{LogError " " + msg + " " + fromWhere};
    AddMessage(log);

#else
    
    std::string log{};
    log.append( RedText +  LogError ResetText " " + msg + " " + fromWhere);
    AddMessage(log);

#endif
}

void powe::ConsoleLogger::LogInfoMessage(const std::string& msg, const std::string& fromWhere, bool)
{
#ifdef _WIN32

    const HANDLE consoleHandle{GetStdHandle(STD_OUTPUT_HANDLE)};
    SetConsoleTextAttribute(consoleHandle, 15);
    const std::string log{LogInfo " " + msg + " " + fromWhere};
    AddMessage(log);

#else
    
    std::string log{};
    log.append( ResetText +  LogInfo ResetText " " + msg + " " + fromWhere);
    AddMessage(log);

#endif
}

void powe::ConsoleLogger::LogWarningMessage(const std::string& msg, const std::string& fromWhere, bool)
{
#ifdef _WIN32

    const HANDLE consoleHandle{GetStdHandle(STD_OUTPUT_HANDLE)};
    SetConsoleTextAttribute(consoleHandle, 14); // yellow text
    const std::string log{LogWarning " " + msg + " " + fromWhere};
    AddMessage(log);
    // log.append(" " + msg + " " + fromWhere + '\n');


#else
    std::string log{};
    log.append( YellowText +  LogWarning ResetText " " + msg + " " + fromWhere);
    AddMessage(log);
#endif
}

powe::ConsoleLogger::~ConsoleLogger()
{
    // notify the thread so we can exit
    //LogInfoMessage("Message Thread Exiting");

#ifdef _WIN32

    const std::string exit{LogInfo " Message Thread Exiting\n"};
    const HANDLE consoleHandle{GetStdHandle(STD_OUTPUT_HANDLE)};
    SetConsoleTextAttribute(consoleHandle, FOREGROUND_GREEN);
    AddMessage(exit);

#else
    
    std::string loggerExitMsg{LogInfo ResetText " " + exit};
    log.append( GreenText + exit );

#endif

    m_IsContextExit = true;
}

void powe::ConsoleLogger::Run()
{
    // this is single a producer so we don't need atomic bool check or anything
    while (!m_IsContextExit)
    {
        std::optional<std::string> msg{};

        {
            std::unique_lock lock{m_Mutex};

            m_TaskWait.wait(lock, [this]()
            {
                return !m_MessageQueue.Empty();
            });
        }

        msg = m_MessageQueue.PopReturn();

        if (msg.has_value())
            std::cout << msg.value() << '\n';
    }

    std::cout << LogInfo " Message Thread exit successfully\n";
    // std::cout << GreenText LogInfo ResetText " Message Thread exit successfully\n";
}

//void powe::ConsoleLogger::DisplayConsoleMessage(const std::string& msg)
//{
//	std::cout << msg << '\n';
//}

//void powe::Logger::DisplayConsoleMessage(const std::string& msg) const
//{
//	std::cout << msg << '\n';
//}
