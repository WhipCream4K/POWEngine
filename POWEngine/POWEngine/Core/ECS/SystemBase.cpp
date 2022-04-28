#include "pch.h"
#include "SystemBase.h"

#include "POWEngine/Core/ECS/Archetype.h"

powe::SystemBase::SystemBase()
	: m_World()
	, m_Keys()
	, m_UpdateCountPerArchetype()
{
}

void powe::SystemBase::InternalUpdate(const Archetype& archetype, float deltaTime)
{
	m_UpdateCountPerArchetype = 0;
	m_CurrentArchetype = &archetype;

	for (const auto gameObjectId : archetype.GameObjectIds)
	{
		OnUpdate(deltaTime, gameObjectId);
		++m_UpdateCountPerArchetype;
	}
}

void powe::SystemBase::SetWorld(WorldEntity* world)
{
	m_World = world;
}

