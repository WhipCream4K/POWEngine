#pragma once

#include <poweCustomtype.h>

#include "PrefabDesc.h"

class SceneFactory;

namespace powe
{
	class WorldEntity;
	class GameObject;
}

class Ingredient
{
public:

	static SharedPtr<powe::GameObject> Create(powe::WorldEntity&,const SharedPtr<SceneFactory>& refScene,const IngredientsDesc& desc);
};

