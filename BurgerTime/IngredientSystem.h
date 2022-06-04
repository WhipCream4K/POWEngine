#pragma once

#include <poweSystem.h>

class IngredientSystem : public powe::SystemBase
{
public:

	IngredientSystem();

protected:

	void OnUpdate(float, powe::GameObjectID) override;
};

