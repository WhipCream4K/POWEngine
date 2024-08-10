#pragma once

#include "Core/Application/Application.h"
#include "Utils/ServiceLocator.h"


#if defined(RELEASE_WITH_LOGINFO) || defined(_DEBUG)

#define POWE_LOG(MSG)	        Application::Get().GetAppService<Logger>()->LogLevel(LogSeverity::Info,MSG," at " __FUNCTION__)
#define POWE_LOGERROR(MSG)		Application::Get().GetAppService<Logger>()->LogLevel(LogSeverity::Error,MSG," at " __FUNCTION__)
#define POWE_LOGWARNING(MSG)	Application::Get().GetAppService<Logger>()->LogLevel(LogSeverity::Warning,MSG," at " __FUNCTION__)

#else

#define POWE_LOG(Serverity,MSG)
//#define POWLOGINFO(MSG)     MSG
#define POWE_LOGERROR(MSG)    MSG
//#define POWLOGWARNING(MSG)  MSG

#endif