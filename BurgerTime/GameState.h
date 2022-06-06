#pragma once

#include <poweCustomtype.h>


struct DynamicSceneData;

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
	static SharedPtr<GameState> ChangingLevel;

	//virtual SharedPtr<GameState> HandleInput(powe::WorldEntity&, powe::GameObjectID) { return {}; }
	virtual SharedPtr<GameState> HandleInput(powe::WorldEntity&,DynamicSceneData*, powe::GameObjectID) { return {}; }
	virtual void Update(powe::WorldEntity&, float,DynamicSceneData*, powe::GameObjectID) {}
	virtual void Enter(powe::WorldEntity&, DynamicSceneData*, powe::GameObjectID) {}
	virtual void Exit(powe::WorldEntity&, DynamicSceneData*, powe::GameObjectID) {}

	GameState() = default;
	virtual ~GameState() = default;
};

