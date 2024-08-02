#pragma once

//#include "POWEngine/Utils/MacroDef.h"
//#include "POWEngine/Services/ServiceLocator.h"

#include "Core/Application/Application.h"
#include "Utils/ServiceLocator.h"
#include "Utils/MacroDef.h"

#if defined(RELEASE_WITH_LOGINFO) || defined(_DEBUG)

#define POWLOGNORMAL(MSG)	Applicateion::Get().GetServiceLocator()<Logger().Log(MSG)
#define POWLOGINFO(MSG)		powe::ServiceLocator::GetLogger().LogInfoMessage(MSG,"at " __FUNCTION__)
#define POWLOGERROR(MSG)	powe::ServiceLocator::GetLogger().LogErrorMessage(MSG, "at " __FUNCTION__) 
#define POWLOGWARNING(MSG)	powe::ServiceLocator::GetLogger().LogWarningMessage(MSG, "at " __FUNCTION__)

#else

#define POWLOGNORMAL(MSG)	MSG
#define POWLOGINFO(MSG)     MSG
#define POWLOGERROR(MSG)    MSG
#define POWLOGWARNING(MSG)  MSG

#endif