#include "pch.h"
#include "ECSManager.h"

powe::ECSManager::ECSManager(std::pmr::memory_resource* memResource)
	: m_Archetypes{ memResource }
{
}
