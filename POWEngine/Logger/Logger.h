#pragma once

#include <format>
#include <source_location>

#include "Core/IModule.h"
#include "Core/Application/Application.h"
#include "Logger/BaseLogger.h"


namespace powe
{
    class BaseLogger;
    class Logger : public IModule
    {
    public:

        Logger();
        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;
        Logger(Logger&&) noexcept = default;
        Logger& operator=(Logger&&) noexcept = default;
        ~Logger() override = default;

        void OnStartUp(ModulesManager*) override;
        void OnExit(ModulesManager*) override;

        SharedPtr<PMRResource> GetResource() const noexcept;
        BaseLogger* GetLogSubsystem() const { return m_LogSubsystem.get(); }
    
    private:

        template<typename... Args>
		static inline std::function<std::string()> LogFromWhere(std::string_view message,const std::source_location& loc, Args&&... args)
		{
            std::function<std::string()> fullMessage = [message, loc, ...args = std::forward<Args>(args)]() 
            { 
			    return std::vformat(message, std::make_format_args(std::forward<Args>(args)...)) + 
                " from: " + loc.file_name() + 
                " at: " + loc.function_name();
            };
            return fullMessage;
		}

        UniquePtr<BaseLogger> m_LogSubsystem;

    };

    template<typename... Args>
    static inline void Info(const std::string& message, Args&&... args)
    {
        auto logModule{Application::GetModule<powe::Logger>()};
        if(!logModule)
            return;

        logModule->GetLogSubsystem()->LogLevel(LogSeverity::Info, 
         Logger::LogFromWhere(message, std::source_location::current(), std::forward<Args>(args)...));
    }

    template<typename... Args>
    static inline void Warning(const std::string& message, Args&&... args)
    {
        auto logModule{Application::GetModule<powe::Logger>()};
        if(!logModule)
            return;

        logModule->GetLogSubsystem()->LogLevel(LogSeverity::Warning,    
         Logger::LogFromWhere(message, std::source_location::current(), std::forward<Args>(args)...));
    }

    template<typename... Args>
    static inline void Error(const std::string& message, Args&&... args)
    {
        auto logModule{Application::GetModule<powe::Logger>()};
        if(!logModule)
            return;

        logModule->GetLogSubsystem()->LogLevel(LogSeverity::Error,
         Logger::LogFromWhere(message, std::source_location::current(), std::forward<Args>(args)...));
    }

}