#include "ScorableEvent.h"

//void ScorableEvent::SignalScore(powe::WorldEntity& worldEntity,EventType type, const Event& data, int playerIndex)
//{
//	NotifyFromPlayer(worldEntity,)
//}

void ScorableEvent::SignalScore(powe::WorldEntity& worldEntity, ScoreType type, const Event& , int playerIndex)
{
	BurgerEvent burgerEvent{};
	switch (type)
	{
	case DropOnPlatform:	burgerEvent = BurgerEvent::IngredientDropToPlatform; break;
	case EnemyHit:			burgerEvent = BurgerEvent::EnemySquash;			break;
	default: ;
	}

	Notify(worldEntity, burgerEvent);
	NotifyFromPlayer(worldEntity, burgerEvent, playerIndex);
}
