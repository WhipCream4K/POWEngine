#pragma once

#include <poweCustomtype.h>
#include "POWEngine/Math/Math.h"

namespace powe
{
	class WorldEntity;
	class GameObject;
}

class BurgerLevel
{
public:

	static SharedPtr<powe::GameObject> Create(powe::WorldEntity& worldEntity, const glm::fvec2& position,int currentLevel);

	// Create ingredients and put it to place using the data from spreadsheets
	static std::vector<SharedPtr<powe::GameObject>> InitializeSubGameObjects(powe::WorldEntity& worldEntity,int currentLevel);
};

