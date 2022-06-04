#pragma once

#include <poweCustomtype.h>
#include "PrefabDesc.h"

namespace powe
{
	class GameObject;
	class WorldEntity;
}

class IngredientDynamic
{
public:

	static SharedPtr<powe::GameObject> Create(powe::WorldEntity& worldEntity,const IngredientDynamicDesc& desc);
};

