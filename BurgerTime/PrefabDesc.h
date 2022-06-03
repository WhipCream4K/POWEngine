#pragma once

#include <poweCustomtype.h>
#include "StaticVariables.h"

struct IngredientsDesc
{
	int currentLevel{};
	powe::GameObjectID colliderManager{};
	IngredientsType ingredientsType{};
};