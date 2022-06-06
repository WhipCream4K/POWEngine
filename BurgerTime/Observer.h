#pragma once

#include <poweCustomtype.h>
#include "BurgerEvent.h"

namespace powe
{
	class WorldEntity;
}

class Subject;
class Observer
{
public:

	virtual void OnReceiveMessage(powe::WorldEntity&,BurgerEvent) = 0;
	virtual void OnReceiveMessageByPlayer(powe::WorldEntity&, BurgerEvent, int) {}

	Observer() = default;
	virtual ~Observer() = default;
};

