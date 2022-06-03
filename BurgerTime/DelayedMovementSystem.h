#pragma once

#include <poweSystem.h>

class DelayedMovementSystem : public powe::SystemBase
{
public:

	DelayedMovementSystem();

protected:

	void OnUpdate(float, powe::GameObjectID) override;
};

