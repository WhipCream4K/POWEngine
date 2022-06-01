#include "GameStart.h"

SharedPtr<GameState> GameStart::HandleInput(powe::WorldEntity& , powe::GameObjectID )
{
	return GameState::MenuState;
}
