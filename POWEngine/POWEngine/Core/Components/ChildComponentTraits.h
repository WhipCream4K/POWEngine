#pragma once

#include "POWEngine/ECS/ECSTypes.h"

namespace powe
{
	struct ChildComponentTraits
	{
		ComponentTypeID ChildComponentID{};
		GameObjectID ParentID{};
		bool isDirty{};
	};
}


