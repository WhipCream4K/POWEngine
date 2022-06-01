#pragma once

#include "Observer.h"
#include "StaticVariables.h"

class PlayModeSubject;
class PlayModeObserver : public Observer
{
public:

	PlayModeObserver(powe::GameObjectID owner);
	void OnReceiveMessage(powe::WorldEntity&, Subject*) override;
	~PlayModeObserver() override = default;

private:

	powe::GameObjectID m_Owner;
};

