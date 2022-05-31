#include "PlayScene.h"

#include "AnimationSystem.h"
#include "POWEngine/Core/GameObject/GameObject.h"
#include "POWEngine/Core/WorldEntity/WorldEntity.h"
#include "BurgerTimeComponents.h"
#include "BurgerLevel.h"
#include "Player.h"


PlayScene::PlayScene(const SharedPtr<powe::GameObject>& dynamicScene)
	: m_SceneData(dynamicScene)
{
}

void PlayScene::LoadScene(powe::WorldEntity& worldEntity)
{
	// Spawn level
	const auto sceneData{ m_SceneData.lock() };
	if (!sceneData)
		return;

	const DynamicSceneData* mainSceneData{ worldEntity.FindUniqueComponent<DynamicSceneData>() };
	if (!mainSceneData)
		return;

	constexpr  glm::fvec2 midScreen{ 640.0f,360.0f };

	const auto level{ BurgerLevel::Create(worldEntity,midScreen,mainSceneData->currentLevel) };
	AddGameObject(level);

	// Spawn player(s)
	switch (mainSceneData->currentPlayMode)
	{
	case PlayMode::SinglePlayer:
	{
		const auto player{ Player::Create(worldEntity,midScreen,mainSceneData->currentLevel) };
		AddGameObject(player);
	}
	break;
	default:;
	}

	// Register System
	const auto animationSystem{ std::make_shared<AnimationSystem>() };
	worldEntity.RegisterSystem(PipelineLayer::PostUpdate, animationSystem);
	AddSystem(animationSystem);
}
