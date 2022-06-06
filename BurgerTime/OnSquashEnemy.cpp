#include "OnSquashEnemy.h"


void OnSquashEnemy::SignalSquashEnemy(powe::WorldEntity& worldEntity, EnemyType enemy, int playerIndex)
{
	BurgerEvent type{};
	switch (enemy)
	{
	case EnemyType::HotDog: type = BurgerEvent::HotDogDead; break;
	default: ;
	}

	NotifyFromPlayer(worldEntity, type, playerIndex);
}
