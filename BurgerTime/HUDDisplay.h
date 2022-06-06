#pragma once

#include "Observer.h"

struct DisplayManager;

class HUDDisplay : public Observer
{
public:

	HUDDisplay() = default;
	HUDDisplay(powe::GameObjectID displayManager);
	void OnReceiveMessageByPlayer(powe::WorldEntity&, BurgerEvent, int) override;
	void OnReceiveMessage(powe::WorldEntity&, BurgerEvent) override;

private:

	void HandlePlayerHealth(powe::WorldEntity&,DisplayManager*,int playerIndex) const;

	powe::GameObjectID m_DisplayManager{};
};

