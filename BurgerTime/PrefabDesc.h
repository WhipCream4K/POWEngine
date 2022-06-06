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

struct PlateDesc
{
	powe::GameObjectID colliderManager{};
	glm::fvec2 position{};
	glm::fvec2 size{};
	int stackNeeded{};
	int currentLevel{};
};

struct IngameHUDDesc
{
	PlayMode playMode{};
	int startPepper{};
	int startLives{};
};

struct EnemyDesc
{
	powe::GameObjectID colliderManager{};
	EnemyType type{};
	glm::fvec2 position{};
};

struct PepperDesc
{
	powe::GameObjectID colliderManager{};
	glm::fvec2 position{};
};
