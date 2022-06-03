#include "PlayState.h"

#include "BurgerTimeComponents.h"
#include "PlayScene.h"


SharedPtr<GameState> PlayState::HandleInput(powe::WorldEntity&, powe::GameObjectID)
{
	return GameState::PlayState;
}

void PlayState::Enter(powe::WorldEntity& worldEntity, powe::GameObjectID gameObjectId)
{
	worldEntity;
	gameObjectId;

	if (!m_PlayScene)
	{
		m_PlayScene = std::make_shared<PlayScene>(gameObjectId);
	}

	m_PlayScene->LoadScene(worldEntity);
}

void PlayState::Exit(powe::WorldEntity& worldEntity, powe::GameObjectID)
{
	if (m_PlayScene)
		m_PlayScene->UnloadScene(worldEntity);
}
