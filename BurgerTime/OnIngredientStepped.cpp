#include "OnIngredientStepped.h"

#include "POWEngine/Core/WorldEntity/WorldEntity.h"

void OnIngredientStepped::SignalStep(powe::WorldEntity& worldEntity)
{
	Notify(worldEntity,BurgerEvent::StepOnIngredients);
}
