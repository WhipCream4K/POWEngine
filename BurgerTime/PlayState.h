#pragma once

#include "GameState.h"

class PlayScene;
class PlayState : public GameState
{
public:

	SharedPtr<GameState> HandleInput(powe::WorldEntity&, powe::GameObjectID) override;
	void Enter(powe::WorldEntity&, powe::GameObjectID) override;
	void Exit(powe::WorldEntity&, powe::GameObjectID) override;

private:

	SharedPtr<PlayScene> m_PlayScene;
};

