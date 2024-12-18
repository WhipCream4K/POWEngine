#include "pch.h"
#include "ComponentInfo.h"

#include "ECSTypes.h"

using namespace powe;

std::atomic<powe::ComponentID> powe::ComponentInfo::m_Counter{0};
powe::UnOrderedMap<powe::ComponentID, size_t> powe::ComponentInfo::m_IDToComponentSize{}; 
UnOrderedMap<ComponentID,std::function<void(void*,void*)>> ComponentInfo::m_IDToMoveOp{};
UnOrderedMap<ComponentID,std::function<void(void*)>> ComponentInfo::m_IDToDestroyOp{};