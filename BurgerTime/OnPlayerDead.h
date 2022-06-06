#pragma once
#include "Subject.h"
class OnPlayerDead : public Subject
{
public:

	void SignalDead(powe::WorldEntity& worldEntity, int playerIndex);
};

