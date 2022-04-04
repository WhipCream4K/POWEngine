#include "pch.h"
#include "System.h"

void powe::System::InternalUpdate(const Archetype& archetype, float deltaTime)
{
	for (const auto& gameID : archetype.GameObjectIds)
	{
		OnUpdate(deltaTime,gameID);
	}
}
