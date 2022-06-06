#pragma once

#include "Observer.h"

class PlaySoundOnEvent : public Observer
{
public:

	PlaySoundOnEvent() = default;
	PlaySoundOnEvent(powe::GameObjectID audioManager);
	void OnReceiveMessage(powe::WorldEntity&, BurgerEvent) override;
	void OnReceiveMessageByPlayer(powe::WorldEntity&, BurgerEvent, int) override;

private:

	powe::GameObjectID m_AudioManager{};
};

