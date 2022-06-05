#include "SquashEnemyListener.h"
#include "OnSquashEnemy.h"

void SquashEnemyListener::OnReceiveMessage(powe::WorldEntity& worldEntity, Subject* subject)
{
	if(const auto some = dynamic_cast<OnSquashEnemy*>(subject))
	{
		OnReceiveSquashEnemy(worldEntity,some->GetSquashEnemy());
	}
}
