#include "OnSquashEnemy.h"


void OnSquashEnemy::SignalSquashEnemy(powe::WorldEntity& worldEntity, EnemyType enemy)
{
	m_SquashEnemy = enemy;
	Notify(worldEntity);
}
