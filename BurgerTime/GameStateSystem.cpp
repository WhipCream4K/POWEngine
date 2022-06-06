#include "GameStateSystem.h"

#include "GameState.h"
#include "BurgerTimeComponents.h"

GameStateSystem::GameStateSystem()
{
	DEFINE_SYSTEM_KEY(DynamicSceneData);
}

void GameStateSystem::OnUpdate(float deltaTime, powe::GameObjectID id)
{
	using namespace powe;

	DynamicSceneData* sceneData = GetComponent<DynamicSceneData>();

	WorldEntity& worldEntity = *GetWorld();

	if(sceneData->currentGameState)
	{
		const auto oldState{ sceneData->currentGameState };
		const auto newState = sceneData->currentGameState->HandleInput(worldEntity, sceneData, id);

		if(oldState != newState)
		{
			oldState->Exit(worldEntity, sceneData, id);
			newState->Enter(worldEntity, sceneData, id);
		}

		sceneData->currentGameState->Update(worldEntity, deltaTime, sceneData, id);

		sceneData->currentGameState = newState;
	}
}
