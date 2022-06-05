#include "OnPlayerThrowPepper.h"

void OnPlayerThrowPepper::SignalThrowPepper(powe::WorldEntity& worldEntity)
{
	Notify(worldEntity);
}
