#pragma once

#include "Observer.h"

class OnPlayerThrowPepper;
class PepperThrowListener : public Observer
{
public:

	PepperThrowListener() = default;
	void OnReceiveMessage(powe::WorldEntity&, Subject*) override;
	virtual void OnReceiveThrowPepper(powe::WorldEntity&) {}
};

