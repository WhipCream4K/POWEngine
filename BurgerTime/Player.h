#pragma once

#include <poweCustomtype.h>
#include "POWEngine/Math/Math.h"

namespace powe
{
	class GameObject;
	class WorldEntity;
}

class Player
{
public:

	static SharedPtr<powe::GameObject> Create(powe::WorldEntity& worldEntity,
		const glm::fvec2& position,int currentLevel);

};

