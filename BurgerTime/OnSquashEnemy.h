#pragma once

#include "Subject.h"

#include "StaticVariables.h"

class OnSquashEnemy : public Subject
{
public:

	void SignalSquashEnemy(powe::WorldEntity&,EnemyType enemy,int playerIndex = 0);

};

