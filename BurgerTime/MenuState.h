#pragma once

#include "GameState.h"

class MenuScene;

class MenuState : public GameState
{
public:

	SharedPtr<GameState> HandleInput(powe::WorldEntity&, powe::GameObjectID) override;
	void Enter(powe::WorldEntity&, powe::GameObjectID) override;
	void Exit(powe::WorldEntity&, powe::GameObjectID) override;

private:

	SharedPtr<MenuScene> m_MenuScene;
};

