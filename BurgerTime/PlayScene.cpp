#include "PlayScene.h"

#include "AnimationSystem.h"
#include "POWEngine/Core/GameObject/GameObject.h"
#include "POWEngine/Core/WorldEntity/WorldEntity.h"
#include "BurgerTimeComponents.h"
#include "BurgerLevel.h"
#include "MovementBlocking.h"
#include "Player.h"
#include "DelayedMovementSystem.h"

#include "StaticSceneData.h"
#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Rendering/Components/Debug/DebugRectangle.h"
#include "FallingSystem.h"
#include "IngredientSystem.h"

PlayScene::PlayScene(powe::GameObjectID sceneGameObject)
	: m_SceneDataID(sceneGameObject)
{
}

void PlayScene::LoadScene(powe::WorldEntity& worldEntity)
{
	using namespace powe;

	GameObjectID dynamicSceneOwner{};
	const DynamicSceneData* mainSceneData{ worldEntity.FindUniqueComponent<DynamicSceneData>(dynamicSceneOwner) };
	if (!mainSceneData)
		return;

	// Spawn level
	const auto& staticSceneData{ Instance<StaticSceneData>() };

	constexpr  glm::fvec2 midScreen{ 640.0f,360.0f };
	const LevelData levelData{ staticSceneData->GetLevelData(mainSceneData->currentLevel) };

	// Initialize Main level
	{
		const auto level{ BurgerLevel::Create(worldEntity,midScreen,mainSceneData->currentLevel) };
		AddGameObject(level);

		// TESTING
		//TileData tileData = staticSceneData->GetSingleTile(mainSceneData->currentLevel, 6, 5);
		//IngredientsDesc ingDesc{mainSceneData->currentLevel,m_SceneDataID,IngredientsType::Lettuce,tileData.position};
		//const auto& subGameObject = IngredientsStatic::Create(worldEntity, ingDesc);

		const LevelDesc levelDesc{ mainSceneData->currentLevel,m_SceneDataID };
		const auto& subGameObjects= BurgerLevel::CreaetStaticIngredients(
			worldEntity, shared_from_this(),levelDesc);

		for (const auto& gb : subGameObjects)
		{
			AddGameObject(gb);
		}
	}

	// Spawn main tile object
	//const auto mainTileObject{ std::make_shared<GameObject>(worldEntity) };
	//AddGameObject(mainTileObject);

	const auto& tileInLevel{ staticSceneData->GetAllTileInLevel(mainSceneData->currentLevel) };

	const bool shouldDebug{ true };

	if(shouldDebug)
	{
		const glm::uvec4 ladder{ 255,105,180,255 };
		const glm::uvec4 none{ 255,0,0,255 };
		const glm::uvec4 platform{ 0,255,0,255 };

		const glm::fvec2 insetBoxSize{ 4.0f,4.0f };

		for (const auto& tile : tileInLevel)
		{
			const auto debugTile{ std::make_shared<GameObject>(worldEntity) };
			AddGameObject(debugTile);

			Transform2D* transform2D = debugTile->AddComponent(Transform2D{ debugTile });
			transform2D->SetWorldPosition(tile.relativePos * burger::SpriteScale + midScreen);
			DebugRectangle* debugRectangle = debugTile->AddComponent(DebugRectangle{ debugTile });
			debugRectangle->SetSize(tile.size - (insetBoxSize * burger::SpriteScale));

			glm::uvec4 outlineColor{};
			switch (tile.tileType)
			{
			case TileType::None:		outlineColor = none; break;
			case TileType::Platform:	outlineColor = platform; break;
			case TileType::Ladder:		outlineColor = ladder;	break;
			default:;
			}

			debugRectangle->SetOutlineColor(outlineColor);
		}
	}

	// Spawn player(s)
	switch (mainSceneData->currentPlayMode)
	{
	case PlayMode::SinglePlayer:
	{
		const PlayerDescriptor player1Desc{
			levelData.playerSpawnPoints * burger::SpriteScale + midScreen,
			mainSceneData->currentLevel,0,m_SceneDataID };

		const auto player{ Player::Create(worldEntity,player1Desc) };
		AddGameObject(player);
	}
	break;
	default:;
	}

	// Register System
	const auto animationSystem{ std::make_shared<AnimationSystem>() };
	worldEntity.RegisterSystem(PipelineLayer::PostUpdate, animationSystem);
	AddSystem(animationSystem);

	const auto movementBlocking{ std::make_shared<MovementBlocking>(mainSceneData->currentLevel) };
	worldEntity.RegisterSystem(PipelineLayer::PostUpdate, movementBlocking);
	AddSystem(movementBlocking);

	const auto ingredientSystem{ std::make_shared<IngredientSystem>() };
	worldEntity.RegisterSystem(PipelineLayer::Update, ingredientSystem);
	AddSystem(ingredientSystem);
}
