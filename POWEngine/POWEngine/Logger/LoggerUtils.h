#pragma once

#include "POWEngine/Services/ServiceLocator.h"
//#include "POWEngine/Logger/ConsoleLogger.h"

#if defined(_DEBUG) || defined(RELEASE_WITH_LOGINFO)

#define POWLOGINFO(MSG)		powe::ServiceLocator::GetLogger().LogInfoMessage(MSG,"at " __FUNCTION__)
#define POWLOGERROR(MSG)	powe::ServiceLocator::GetLogger().LogErrorMessage(MSG, "at " __FUNCTION__) 
#define POWLOGWARNING(MSG)	powe::ServiceLocator::GetLogger().LogWarningMessage(MSG, "at " __FUNCTION__) 

#else

#define POWLOGINFO(MSG)
#define POWLOGERROR(MSG)
#define POWLOGWARNING(MSG)

#endif