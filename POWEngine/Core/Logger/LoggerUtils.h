#pragma once

#include "Core/Application/Application.h"
#include "Utils/ServiceLocator.h"


#if defined(RELEASE_WITH_LOGINFO) || defined(_DEBUG)

#define POWE_LOG(Severity,MSG)	Application::Get().GetServiceLocator().GetService<Logger>()->LogLevel(Severity,MSG," at " __FUNCTION__)
//#define POWE_LOGINFO(MSG)		powe::ServiceLocator::GetLogger().LogInfoMessage(MSG,"at " __FUNCTION__)
//#define POWE_LOGERROR(MSG)	powe::ServiceLocator::GetLogger().LogErrorMessage(MSG, "at " __FUNCTION__) 
//#define POWE_OGWARNING(MSG)	powe::ServiceLocator::GetLogger().LogWarningMessage(MSG, "at " __FUNCTION__)

#else

#define POWE_LOG(Serverity,MSG)
//#define POWLOGINFO(MSG)     MSG
//#define POWLOGERROR(MSG)    MSG
//#define POWLOGWARNING(MSG)  MSG

#endif