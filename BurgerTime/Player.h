#pragma once

#include <poweCustomtype.h>
#include "POWEngine/Math/Math.h"
#include "StaticVariables.h"

namespace powe
{
	class GameObject;
	class WorldEntity;
}

class Player
{
public:

	static SharedPtr<powe::GameObject> Create(powe::WorldEntity& worldEntity,
		const PlayerDescriptor& playerDescriptor);

};

