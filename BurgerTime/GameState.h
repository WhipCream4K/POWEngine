#pragma once

#include <poweCustomtype.h>



namespace powe
{
	class WorldEntity;
}

class MenuState;
class PlayState;
class GameState
{
public:

	static SharedPtr<GameState> PlayState;
	static SharedPtr<GameState> MenuState;
	static SharedPtr<GameState> GameStart;

	virtual SharedPtr<GameState> HandleInput(powe::WorldEntity&, powe::GameObjectID) { return {}; }
	virtual void Update(powe::WorldEntity&, float, powe::GameObjectID) {}
	virtual void Enter(powe::WorldEntity&, powe::GameObjectID) {}
	virtual void Exit(powe::WorldEntity&, powe::GameObjectID) {}

	GameState() = default;
	virtual ~GameState() = default;
};

