#include "OnPlayerThrowPepper.h"


void OnPlayerThrowPepper::SignalThrowPepper(powe::WorldEntity& worldEntity, int playerIndex)
{
	NotifyFromPlayer(worldEntity, BurgerEvent::ThrowPepper,playerIndex);
}
