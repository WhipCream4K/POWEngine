#pragma once

#include "Subject.h"

#include "StaticVariables.h"

class OnSquashEnemy : public Subject
{
public:

	void SignalSquashEnemy(powe::WorldEntity&,EnemyType enemy);
	EnemyType GetSquashEnemy() const { return m_SquashEnemy; }

private:

	EnemyType m_SquashEnemy{};
};

