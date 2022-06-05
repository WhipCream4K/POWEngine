#pragma once

#include "Observer.h"
#include "StaticVariables.h"

class OnSquashEnemy;
class SquashEnemyListener : public Observer
{

public:

	SquashEnemyListener() = default;
	void OnReceiveMessage(powe::WorldEntity&, Subject*) override;
	virtual void OnReceiveSquashEnemy(powe::WorldEntity& ,EnemyType) {}
};

