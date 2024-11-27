#pragma once


#include <functional>

namespace powe
{

	enum class LogSeverity
	{
		Info,
		Warning,
		Error
	};

	class BaseLogger
	{
	public:

		BaseLogger() = default;
		BaseLogger(const BaseLogger&) = default;
		BaseLogger& operator=(const BaseLogger&) noexcept = default;
		BaseLogger(BaseLogger&&) = default;
		BaseLogger& operator=(BaseLogger&&) noexcept = default;
		virtual ~BaseLogger() = default;

		virtual void LogLevel(LogSeverity severity, const std::function<std::string()>& format) = 0;

	};


}


