#pragma once

namespace powe
{

	enum class LogSeverity
	{
		Info,
		Warning,
		Error
	};

	class Logger
	{
	public:

		Logger() = default;
		Logger(const Logger&) = delete;
		Logger& operator=(const Logger&) = delete;
		Logger(Logger&&) = delete;
		Logger& operator=(Logger&&) = delete;
		virtual ~Logger() = default;

		std::string ServiceType() const { return "Logger"; }
		virtual void LogLevel(LogSeverity severity, const std::string& message, const std::string& fromWhere = "") = 0;
		virtual void Log(const std::string& message) = 0;
	};
}


