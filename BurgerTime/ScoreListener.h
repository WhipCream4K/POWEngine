#pragma once
#include "Observer.h"


class ScoreListener : public Observer
{
public:

	ScoreListener(powe::GameObjectID displayManager);
	void OnReceiveMessage(powe::WorldEntity&, BurgerEvent) override;
	void OnReceiveMessageByPlayer(powe::WorldEntity&, BurgerEvent, int) override;

private:

	powe::GameObjectID m_DisplayManager{};
};

