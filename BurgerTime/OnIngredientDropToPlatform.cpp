#include "OnIngredientDropToPlatform.h"

void OnIngredientDropToPlatform::SignalFinishedDrop(powe::WorldEntity& worldEntity,int playerIndex)
{
	NotifyFromPlayer(worldEntity, BurgerEvent::IngredientDropToPlatform, playerIndex);
}
