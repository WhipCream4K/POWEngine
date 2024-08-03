#pragma once

#include "Utils/Service.h"

namespace powe
{

	enum class LogSeverity
	{
		Info,
		Warning,
		Error
	};

	class Logger : public IService<Logger>
	{
	public:

		Logger() = default;
		Logger(const Logger&) = default;
		Logger& operator=(const Logger&) noexcept = default;
		Logger(Logger&&) = default;
		Logger& operator=(Logger&&) noexcept = default;
		virtual ~Logger() = default;

		virtual void LogLevel(LogSeverity severity, const std::string& message, const std::string& fromWhere = "") = 0;
		virtual void Log(const std::string& message) = 0;
	};


}


