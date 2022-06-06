#pragma once

#include "GameState.h"

class ChangeLevel : public GameState
{
public:

	SharedPtr<GameState> HandleInput(powe::WorldEntity&, DynamicSceneData*, powe::GameObjectID) override;
	void Enter(powe::WorldEntity&, DynamicSceneData*, powe::GameObjectID) override;
};

