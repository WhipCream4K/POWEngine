#pragma once

#include <poweSystem.h>

class StateController : public powe::SystemBase
{
public:

	StateController();

protected:

	void OnCreate(powe::GameObjectID) override;
	void OnUpdate(float, powe::GameObjectID) override;
};

