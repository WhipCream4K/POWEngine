#pragma once

#include <poweSystem.h>

class GameStateSystem : public powe::SystemBase
{
public:

	GameStateSystem();

protected:

	void OnUpdate(float, powe::GameObjectID) override;
};

