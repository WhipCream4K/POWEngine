#include "PlayState.h"

#include "BurgerTimeComponents.h"
#include "PlayScene.h"
#include "StaticSceneData.h"
#include "POWEngine/Core/WorldEntity/WorldEntity.h"
#include <ranges>

#include "AudioManager.h"
#include "POWEngine/Sound/SoundInfo.h"


SharedPtr<GameState> PlayState::HandleInput(powe::WorldEntity& worldEntity, DynamicSceneData* dynamicSceneData, powe::GameObjectID)
{
	if (dynamicSceneData->intervalPlayState == IntervalPlayState::ShouldChangeLevel)
	{
		// Play the sound for a while
		// remove system that handling the input and movement
		// waits
		// if the sound is finished then go to interval change to next level and come back again
		return ChangingLevel;
	}

	if (worldEntity.GetInputSettings().GetInputAction("SkipStage", InputEvent::IE_Pressed))
	{
		return ChangingLevel;
	}

	if(worldEntity.GetInputSettings().GetInputAction("SkipToMenu",InputEvent::IE_Pressed))
	{
		return MenuState;
	}

	return GameState::PlayState;
}

void PlayState::Enter(powe::WorldEntity& worldEntity, DynamicSceneData* dynamicSceneData, powe::GameObjectID sceneID)
{
	worldEntity;

	using namespace powe;

	// Play Enter sound
	powe::GameObjectID audioManagerObject{};
	AudioManager* audioManager = worldEntity.FindUniqueComponent<AudioManager>(audioManagerObject);
	if (audioManager)
	{
		SoundInfo info{};
		info.pitch = 1.0f;
		info.volume = 0.5f;

		audioManager->PlaySound(BasicSound::GameStart, info);
	}

	const auto& staticSceneData{ Instance<StaticSceneData>() };

	const int plateCountThisLevel{ staticSceneData->GetPlateServingPiecesCount(dynamicSceneData->currentLevel) };

	if (!dynamicSceneData->OnGameWin)
		dynamicSceneData->OnGameWin = std::make_shared<WinConditionListener>(sceneID,audioManagerObject,plateCountThisLevel);
	else
	{
		dynamicSceneData->OnGameWin->Reset();
		dynamicSceneData->OnGameWin->SetMaxServingCount(plateCountThisLevel);
	}

	dynamicSceneData->currentScene = std::make_shared<PlayScene>(sceneID);
	dynamicSceneData->currentScene->LoadScene(worldEntity);

	dynamicSceneData->intervalPlayState = IntervalPlayState::Maploaded;
}

void PlayState::Exit(powe::WorldEntity& worldEntity, DynamicSceneData* dynamicSceneData, powe::GameObjectID)
{
	if (dynamicSceneData->currentScene)
		dynamicSceneData->currentScene->UnloadScene(worldEntity);

	//for (const auto& systems : dynamicSceneData->blockingSystem | std::views::values)
	//{
	//	for (const auto& systemBase : systems)
	//	{
	//		// Get rid of moving system so we can paused the game
	//		worldEntity.RemoveSystem(systemBase);
	//	}
	//}

	AudioManager* audioManager = worldEntity.FindUniqueComponent<AudioManager>();
	if (audioManager)
	{
		audioManager->StopSound(BasicSound::BGM);
	}
}

void PlayState::Update(powe::WorldEntity& worldEntity, float deltaTime, DynamicSceneData* dynamicSceneData,
	powe::GameObjectID)
{

	switch (dynamicSceneData->intervalPlayState)
	{
	case IntervalPlayState::Maploaded:

		if (dynamicSceneData->gameStartTimeCounter < dynamicSceneData->timeBeforeGameStart)
		{
			dynamicSceneData->gameStartTimeCounter += deltaTime;
		}
		else
		{
			dynamicSceneData->gameStartTimeCounter -= dynamicSceneData->timeBeforeGameStart;
			dynamicSceneData->intervalPlayState = IntervalPlayState::Playing;

			AudioManager* audioManager = worldEntity.FindUniqueComponent<AudioManager>();
			if (audioManager)
			{
				powe::SoundInfo info{};
				info.pitch = 1.0f;
				info.volume = 0.5f;
				audioManager->PlaySound(BasicSound::BGM, info);
			}

			EnableInput(worldEntity, dynamicSceneData);

			dynamicSceneData->intervalPlayState = IntervalPlayState::Playing;

		}

		break;
	case IntervalPlayState::LevelWin:

		// Count down to change the level
		if (dynamicSceneData->gameStartTimeCounter < dynamicSceneData->timeBeforeGameStart)
		{
			dynamicSceneData->gameStartTimeCounter += deltaTime;
		}
		else
		{
			dynamicSceneData->gameStartTimeCounter -= dynamicSceneData->timeBeforeGameStart;
			dynamicSceneData->intervalPlayState = IntervalPlayState::ShouldChangeLevel;
		}

		break;
	case IntervalPlayState::ShouldChangeLevel: break;
	case IntervalPlayState::Playing: break;
	default:;
	}
}

void PlayState::EnableInput(powe::WorldEntity& worldEntity,DynamicSceneData* sceneData) const
{
	for (const auto& [layer,systems] : sceneData->blockingSystem)
	{
		for (const auto& systemBase : systems)
		{
			worldEntity.RegisterSystem(layer, systemBase);
		}
	}
}
