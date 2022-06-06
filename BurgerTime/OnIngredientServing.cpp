#include "OnIngredientServing.h"

#include "POWEngine/Core/WorldEntity/WorldEntity.h"

void OnIngredientServing::SignalServing(powe::WorldEntity& worldEntity)
{
	Notify(worldEntity,BurgerEvent::PlateFull);
}
