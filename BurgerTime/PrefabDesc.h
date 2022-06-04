#pragma once

#include <poweCustomtype.h>
#include "StaticVariables.h"

struct IngredientsDesc
{
	powe::GameObjectID colliderManager{};
	IngredientsType ingredientsType{};
	glm::fvec2 position{};
	int startCol{};
	int startRow{};
	powe::GameObjectID stepHandler{};
};

struct LevelDesc
{
	int currentLevel{};
	powe::GameObjectID colliderManager{};
};

struct IngredientDynamicDesc
{
	IngredientsType type{};
	powe::GameObjectID colliderManager{};
	glm::fvec2 position{};
};
