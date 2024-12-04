#include "pch.h"
#include "ComponentInfo.h"

#include "ECSTypes.h"

std::atomic<powe::ComponentID> powe::ComponentInfo::m_Counter{0};
powe::UnOrderedMap<powe::ComponentID, size_t> powe::ComponentInfo::m_IDToComponentSize{}; 