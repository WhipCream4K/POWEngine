#pragma once

#include "GameState.h"

class GameStart : public GameState
{
public:

	SharedPtr<GameState> HandleInput(powe::WorldEntity&, powe::GameObjectID) override;
};

