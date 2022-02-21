#pragma once

#include "POWEngine/Services/ServiceLocator.h"
#include "POWEngine/Logger/Logger.h"

#if defined(_DEBUG) || defined(RELEASE_WITH_LOGINFO)

#define POWLOGINFO(MSG)		ServiceLocator::GetService<Logger>()->AddInfoMessage(MSG)
#define POWLOGERROR(MSG)	ServiceLocator::GetService<Logger>()->AddErrorMessage(#MSG " at " __FUNCTION__) 
#define POWLOGWARNING(MSG)	ServiceLocator::GetService<Logger>()->AddWarningMessage(MSG " at " __FUNCTION__) 

#else

#define POWLOGINFO(MSG)
#define POWLOGERROR(MSG)
#define POWLOGWARNING(MSG)

#endif