#include "MenuState.h"

#include "BurgerTimeComponents.h"
#include "MenuScene.h"
#include "POWEngine/Core/WorldEntity/WorldEntity.h"
#include "InGameHUD.h"
#include <ranges>

#include "POWEngine/Core/GameObject/GameObject.h"

SharedPtr<GameState> MenuState::HandleInput(powe::WorldEntity& worldEntity, DynamicSceneData*, powe::GameObjectID)
{
	const auto& inputSettings{ worldEntity.GetInputSettings() };
	if (inputSettings.GetInputAction("Select",InputEvent::IE_Pressed))
	{
		return GameState::PlayState;
	}

	return GameState::MenuState;
}

void MenuState::Enter(powe::WorldEntity& worldEntity, DynamicSceneData* dynamicSceneData, powe::GameObjectID sceneID)
{

	dynamicSceneData->currentScene = std::make_shared<MenuScene>(sceneID);
	dynamicSceneData->currentScene->LoadScene(worldEntity);
	dynamicSceneData->currentPlayMode = PlayMode::SinglePlayer;

	worldEntity.GetInputSettings().SetAssignFirstControllerToNextPlayer(false);

	if (dynamicSceneData->DisplayManager)
	{
		// Remove UI elements
		DisplayManager* displayManager = dynamicSceneData->DisplayManager->GetComponent<DisplayManager>();
		if(displayManager)
		{
			for (const auto& healthDisplay : displayManager->healthDisplay)
			{
				if(healthDisplay)
					worldEntity.RemoveGameObject(healthDisplay->GetID());
			}

			for (const auto& banner : displayManager->banner)
			{
				if(banner)
					worldEntity.RemoveGameObject(banner->GetID());
			}

			for (const auto& pepperDisplay : displayManager->pepperText)
			{
				if (pepperDisplay)
					worldEntity.RemoveGameObject(pepperDisplay->GetID());
			}
		}
	}

}

void MenuState::Exit(powe::WorldEntity& worldEntity, DynamicSceneData* dynamicSceneData, powe::GameObjectID)
{
	if(dynamicSceneData->currentScene)
	{
		dynamicSceneData->currentScene->UnloadScene(worldEntity);
	}

	if(!dynamicSceneData->DisplayManager)
	{
		IngameHUDDesc desc{dynamicSceneData->currentPlayMode};
		const auto display = InGameHUD::CreateDisplayManager(worldEntity, desc);
		dynamicSceneData->DisplayManager = display;
	}

	for (const auto& systems : dynamicSceneData->blockingSystem | std::views::values)
	{
		for (const auto& systemBase : systems)
		{
			worldEntity.RemoveSystem(systemBase);
		}
	}

	if(dynamicSceneData->currentPlayMode == PlayMode::Coop)
	{
		worldEntity.GetInputSettings().SetAssignFirstControllerToNextPlayer(true);
	}

}
