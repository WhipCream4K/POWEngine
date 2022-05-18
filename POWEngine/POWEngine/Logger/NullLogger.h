#pragma once

#include "POWEngine/Logger/Logger.h"

namespace powe
{
	class NullLogger final : public Logger
	{
	public:

		NullLogger() = default;
		NullLogger(const NullLogger&) = delete;
		NullLogger& operator=(const NullLogger&) = delete;
		NullLogger(NullLogger&&) = delete;
		NullLogger& operator=(NullLogger&&) = delete;
		~NullLogger() override = default;

	public:

		void LogErrorMessage(const std::string&, const std::string& ) override {}
		void LogInfoMessage(const std::string&, const std::string& ) override {}
		void LogWarningMessage(const std::string&, const std::string& ) override {}
	};
}


