#include "GameStart.h"

SharedPtr<GameState> GameStart::HandleInput(powe::WorldEntity&, DynamicSceneData*, powe::GameObjectID)
{
	return GameState::MenuState;
}
