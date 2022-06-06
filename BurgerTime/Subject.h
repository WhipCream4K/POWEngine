#pragma once

#include <poweCustomtype.h>

#include "BurgerEvent.h"

namespace powe
{
	class WorldEntity;
}

class Observer;
class Subject
{
public:

	void Notify(powe::WorldEntity&,BurgerEvent type);
	void NotifyFromPlayer(powe::WorldEntity&,BurgerEvent type,int playerIndex);
	
	void Attach(Observer* observer);
	void Remove(Observer* observer);

	Subject() = default;
	virtual ~Subject() = default;

private:

	std::vector<Observer*> m_Observers;
};

