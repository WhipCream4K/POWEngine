#pragma once

#include "POWEngine/Core/ECS/ECSTypes.h"

namespace powe
{
	struct ChildComponentTraits
	{
		std::vector<ComponentTypeID> ChildComponentsID{};
		GameObjectId ParentID{};
		GameObjectId SelfID{};
	};
}


