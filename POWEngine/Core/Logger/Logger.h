#pragma once

namespace powe
{

	enum class LogSeverity
	{
		Info,
		Warning,
		Errror
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

		//virtual void LogInfoMessage(const std::string&,const std::string& fromWhere = "",bool showOrigin = false) = 0;
		//virtual void LogWarningMessage(const std::string&, const std::string& fromWhere = "", bool showOrigin = false) = 0;
		//virtual void LogErrorMessage(const std::string&, const std::string& fromWhere = "", bool showOrigin = false) = 0;

		virtual void Log(LogSeverity severity, const std::string& message, const std::string& fromWhere = "") = 0;
		
	};
}


