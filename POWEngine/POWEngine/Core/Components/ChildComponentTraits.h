#pragma once

#include "POWEngine/Core/ECS/ECSTypes.h"

namespace powe
{
	struct ChildComponentTraits
	{
		ComponentTypeID ChildComponentID{};
		GameObjectID ParentID{};
		bool isDirty{};
	};
}


