#pragma once

namespace powe
{
	class Logger
	{
	public:

		Logger() = default;
		Logger(const Logger&) = delete;
		Logger& operator=(const Logger&) = delete;
		Logger(Logger&&) = delete;
		Logger& operator=(Logger&&) = delete;
		virtual ~Logger() = default;

	public:

		virtual void LogInfoMessage(const std::string&,const std::string& fromWhere = "") = 0;
		virtual void LogWarningMessage(const std::string&, const std::string& fromWhere = "") = 0;
		virtual void LogErrorMessage(const std::string&, const std::string& fromWhere = "") = 0;
		
	};
}


