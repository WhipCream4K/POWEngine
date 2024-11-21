#pragma once

#include "Core/Application/Application.h"
#include "Utils/ServiceLocator.h"


#if defined(RELEASE_WITH_LOGINFO) || defined(_DEBUG)

#define POWE_LOG(MSG)	        Application::GetAppService<Logger>()->LogLevel(LogSeverity::Info,MSG, std::string(" at ") + __func__)
#define POWE_LOGERROR(MSG)		Application::GetAppService<Logger>()->LogLevel(LogSeverity::Error,MSG, std::string(" at ") + __func__)
#define POWE_LOGWARNING(MSG)	Application::GetAppService<Logger>()->LogLevel(LogSeverity::Warning,MSG, std::string(" at ") + __func__)

#else

#define POWE_LOG(MSG)
//#define POWLOGINFO(MSG)     MSG
#define POWE_LOGERROR(MSG)    MSG
#define POWLOGWARNING(MSG)  MSG

#endif