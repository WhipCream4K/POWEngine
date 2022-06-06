#pragma once

#include "Observer.h"

class PepperNumberDisplay : public Observer
{
public:

	PepperNumberDisplay() = default;
	PepperNumberDisplay(powe::GameObjectID displayManager);

	void OnReceiveMessageByPlayer(powe::WorldEntity&, BurgerEvent, int) override;
	void OnReceiveMessage(powe::WorldEntity&, BurgerEvent) override;

private:

	powe::GameObjectID m_DisplayManager{};
};

