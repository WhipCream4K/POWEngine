#pragma once

#include "StaticVariables.h"
#include "Subject.h"

class ScorableEvent : public Subject	
{
public:

	enum ScoreType
	{
		DropOnPlatform,
		EnemyHit
	};

	union Event
	{
		EnemyType enemyType{};
	};

	void SignalScore(powe::WorldEntity& worldEntity, ScoreType type, const Event& data,int playerIndex);

};

