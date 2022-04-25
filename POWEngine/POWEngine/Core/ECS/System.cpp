#include "pch.h"
#include "System.h"


void powe::System::InternalUpdate(const Archetype& archetype, float deltaTime)
{
	m_UpdateCount = 0;

	for (const auto& gameID : archetype.GameObjectIds)
	{
		OnUpdate(deltaTime,gameID);
		m_UpdateCount++;
	}
}
