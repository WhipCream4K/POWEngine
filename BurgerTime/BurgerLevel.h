#pragma once

#include <poweCustomtype.h>
#include "POWEngine/Math/Math.h"
#include "IngredientsStatic.h"

class SceneFactory;

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
	//static std::vector<SharedPtr<powe::GameObject>> InitializeSubGameObjects(powe::WorldEntity& worldEntity,int currentLevel);
	static std::vector<SharedPtr<powe::GameObject>> CreaetStaticIngredients(powe::WorldEntity&,const SharedPtr<SceneFactory>& sceneRef,const LevelDesc& levelDesc);
};

