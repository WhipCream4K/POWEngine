#include "pch.h"
#include "GameObject.h"

#include "POWEngine/Core/WorldEntity/WorldEntity.h"

powe::GameObject::GameObject(const SharedPtr<WorldEntity>& world)
	: m_World(world)
	, m_Id()
{
	if (world)
	{
		m_Id = world->GetEntityId();
	}
}
