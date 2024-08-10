#include "pch.h"
#include "ComponentIDGen.h"

#include "ECSTypes.h"

std::atomic<powe::ComponentID> powe::ComponentIDGen::m_Counter{0};