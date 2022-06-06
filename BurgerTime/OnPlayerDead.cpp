#include "OnPlayerDead.h"

#include "POWEngine/Core/WorldEntity/WorldEntity.h"

void OnPlayerDead::SignalDead(powe::WorldEntity& worldEntity,int playerIndex)
{
	NotifyFromPlayer(worldEntity, BurgerEvent::PlayerDead, playerIndex);
}
