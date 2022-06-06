#include "ChangeLevel.h"

#include "BurgerTimeComponents.h"


SharedPtr<GameState> ChangeLevel::HandleInput(powe::WorldEntity& , DynamicSceneData* ,
											  powe::GameObjectID )
{
	return GameState::PlayState;
}

void ChangeLevel::Enter(powe::WorldEntity& , DynamicSceneData* dynamicSceneData,
	powe::GameObjectID )
{
	dynamicSceneData->currentLevel = (dynamicSceneData->currentLevel + 1) % dynamicSceneData->maxLevel;
}
