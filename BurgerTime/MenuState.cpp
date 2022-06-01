#include "MenuState.h"

#include "MenuScene.h"
#include "POWEngine/Core/WorldEntity/WorldEntity.h"

SharedPtr<GameState> MenuState::HandleInput(powe::WorldEntity& worldEntity, powe::GameObjectID)
{
	const auto& inputSettings{ worldEntity.GetInputSettings() };
	if (inputSettings.GetInputAxis("Select") > 0.0f)
	{
		return GameState::PlayState;
	}

	return GameState::MenuState;
}

void MenuState::Enter(powe::WorldEntity& worldEntity, powe::GameObjectID gameObjectId)
{
	if (!m_MenuScene)
	{
		m_MenuScene = std::make_shared<MenuScene>(gameObjectId);
	}

	m_MenuScene->LoadScene(worldEntity);
}

void MenuState::Exit(powe::WorldEntity& worldEntity, powe::GameObjectID )
{
	if (m_MenuScene)	
		m_MenuScene->UnloadScene(worldEntity);

}
