#include "IngredientState.h"

#include "BurgerTimeComponents.h"
#include "IngredientsComponent.h"
#include "IngredientsStatic.h"
#include "SceneFactory.h"
#include "POWEngine/Core/WorldEntity/WorldEntity.h"
#include "POWEngine/Core/GameObject/GameObject.h"
#include "POWEngine/Core/Components/Transform2D.h"
#include "AssetManager.h"
#include "ColliderResolver.h"
#include "StaticSceneData.h"
#include "POWEngine/Rendering/Components/Sprite/SpriteComponent.h"
#include "Rect2DCollider.h"
#include "utils.h"
#include "POWEngine/Rendering/Components/Debug/DebugRectangle.h"

SharedPtr<IngredientState> IngredientState::Stationary{ std::make_shared<StationaryIngredient>() };
SharedPtr<IngredientState> IngredientState::Falling{ std::make_shared<FallingIngredient>() };
SharedPtr<IngredientState> IngredientState::Spawn{ std::make_shared<SpawnIngredient>() };
SharedPtr<IngredientState> IngredientState::Bounce{ std::make_shared<Bouncing>() };

SharedPtr<IngredientState> SpawnIngredient::Update(powe::WorldEntity&, float,
	IngredientsComponent*, powe::GameObjectID)
{
	return Stationary;
}

SharedPtr<IngredientState> StationaryIngredient::Update(powe::WorldEntity&, float,
	IngredientsComponent* ingredientsComponent, powe::GameObjectID)
{
	if (ingredientsComponent->CurrentStepCount >= ingredientsComponent->MaxStepCount)
	{
		return Falling;
	}

	return Stationary;
}

void StationaryIngredient::Enter(powe::WorldEntity& worldEntity, IngredientsComponent* ingredientsComponent, powe::GameObjectID owner)
{
	using namespace powe;

	ingredientsComponent->CurrentStepCount = 0;

	GameObjectID resolverID{};
	ColliderResolver* colliderResolver = worldEntity.FindUniqueComponent<ColliderResolver>(resolverID);
	if (!colliderResolver)
		return;

	Transform2D* transform2D = worldEntity.GetComponent<Transform2D>(owner);
	if (!transform2D)
		return;

	ingredientsComponent->StepPushDecreaseOffset = 2.0f * burger::SpriteScale.y;
	ingredientsComponent->StepPushDistance = 7.0f * burger::SpriteScale.y;

	// Initialize colliders
	const IngredientsDesc desc{ resolverID,ingredientsComponent->Type,transform2D->GetWorldPosition(),0,0,owner };
	auto colldiers{ IngredientsStatic::Create(worldEntity,desc) };

	ingredientsComponent->StepColliders.insert(ingredientsComponent->StepColliders.begin(), colldiers.begin(), colldiers.end());

	if (const auto scene = ingredientsComponent->SceneRef.lock())
	{
		for (const auto& gb : colldiers)
		{
			scene->AddGameObject(gb);
		}
	}
}

void StationaryIngredient::Exit(powe::WorldEntity& worldEntity, IngredientsComponent* ingredientsComponent, powe::GameObjectID)
{
	using namespace powe;

	for (auto& weakGameObject : ingredientsComponent->StepColliders)
	{
		if (const auto collider{ weakGameObject.lock() })
		{
			worldEntity.RemoveGameObject(collider->GetID());

			if (const auto scene{ ingredientsComponent->SceneRef.lock() })
			{
				scene->RemoveGameObject(collider);
			}
		}
	}

	ingredientsComponent->StepColliders.clear();

}

SharedPtr<IngredientState> FallingIngredient::Update(powe::WorldEntity&, float deltaTime,
	IngredientsComponent* ingredientsComponent, powe::GameObjectID)
{
	// Change state when encounter a platform or
	using namespace powe;
	if (const auto gb = ingredientsComponent->Owner.lock())
	{
		Transform2D* transform2D = gb->GetComponent<Transform2D>();
		auto oldPos{ transform2D->GetWorldPosition() };
		//transform2D->SetWorldPosition(oldPos);

		// Check if it travels to another platform yet
		const auto& staticSceneData{ Instance<StaticSceneData>() };
		const auto tileSet{ staticSceneData->GetAllTileInLevel(ingredientsComponent->LevelIdx) };
		const int maxTileCol{ staticSceneData->GetMaxColTile() };
		const int maxTileRow{ staticSceneData->GetMaxRowTile() };
		const int nextRow{ ingredientsComponent->StartRow + 1 };

		if (nextRow >= maxTileRow)
			return Falling;

		const auto& downTile{ tileSet[nextRow * maxTileCol + ingredientsComponent->StartCol] };

		constexpr int downDir{ 1 };

		if (IsOutOfBound(downDir, oldPos.y, downTile.position.y))
		{
			if (downTile.tileType == TileType::Platform || downTile.tileType == TileType::LadderStart)
			{
				// Stop falling
				transform2D->SetWorldPosition(downTile.position);
				++ingredientsComponent->StartRow;
				return Bounce;
			}

			++ingredientsComponent->StartRow;
		}
		else
		{
			oldPos.y += ingredientsComponent->FallingSpeed * deltaTime;
			transform2D->SetWorldPosition(oldPos);
		}

	}


	return Falling;
}

void FallingIngredient::Enter(powe::WorldEntity& worldEntity, IngredientsComponent* ingredientsComponent,
	powe::GameObjectID)
{
	ingredientsComponent;
	using namespace powe;

	// Give the parent a whole sprite of the ingredient
	const auto& assetManager{ Instance<AssetManager>() };
	const auto& mainTexture{ assetManager->GetAsset<Texture>(burger::MainObjectSprite) };

	const auto& spriteInfo{ Instance<StaticSceneData>()->GetIngredientSpriteInfo(ingredientsComponent->Type) };

	if (const auto gameObject = ingredientsComponent->Owner.lock())
	{
		SpriteComponent* spriteComponent = gameObject->AddComponent(SpriteComponent{ gameObject }, ComponentFlag::Sparse);
		spriteComponent->SetTexture(*mainTexture);
		spriteComponent->SetTexture(*mainTexture);
		spriteComponent->SetRect(spriteInfo.rect);
		spriteComponent->SetRenderOrder(burger::RenderOrder::Player);

		Transform2D* transform2D = gameObject->GetComponent<Transform2D>();
		if (transform2D)
		{
			auto oldPos{ transform2D->GetWorldPosition() };
			const float stepOffset{ 10.0f };
			oldPos.y += stepOffset;
			transform2D->SetWorldPosition(oldPos);
			transform2D->SetWorldScale(burger::SpriteScale);
		}

		GameObjectID resolverID{};
		ColliderResolver* colliderResolver = worldEntity.FindUniqueComponent<ColliderResolver>(resolverID);
		if (!colliderResolver)
			return;

		const glm::fvec2 colliderSize{ spriteInfo.rect.z,spriteInfo.rect.w };

		gameObject->AddComponent(Rect2DCollider{
			worldEntity,
			gameObject->GetID(),
			resolverID,
			colliderSize,
			OverlapLayer::Dynamic
			});

#ifdef _DEBUG

		gameObject->AddComponent(DebugRectangle{ gameObject,colliderSize * burger::SpriteScale }, ComponentFlag::Sparse);

#endif

	}
}

void FallingIngredient::Exit(powe::WorldEntity& worldEntity, IngredientsComponent* ingredientsComponent,
	powe::GameObjectID gameObjectId)
{
	worldEntity;
	gameObjectId;
	ingredientsComponent;

	using namespace powe;

		if(const auto gameObject = ingredientsComponent->Owner.lock())
		{
			gameObject->RemoveComponent<Rect2DCollider>();

	#ifdef _DEBUG
	
			gameObject->RemoveComponent<DebugRectangle>();
	
	#endif
		}
}

SharedPtr<IngredientState> Bouncing::Update(powe::WorldEntity& worldEntity, float deltaTime,
	IngredientsComponent* ingredientsComponent, powe::GameObjectID gameObjectId)
{
	using namespace powe;

	Transform2D* transform2D{ worldEntity.GetComponent<Transform2D>(gameObjectId) };
	if (transform2D)
	{
		auto oldPos{ transform2D->GetWorldPosition() };


		if (ingredientsComponent->BounceTimeCounter < ingredientsComponent->BounceApexTime)
		{
			oldPos.y -= ingredientsComponent->BouncingSpeed * deltaTime;
			ingredientsComponent->BounceTimeCounter += deltaTime;
			ingredientsComponent->BouncingSpeed -= 9.81f * deltaTime * 5.0f;
		}
		else
		{
			oldPos.y += ingredientsComponent->BouncingSpeed * deltaTime;
			ingredientsComponent->BouncingSpeed += 9.81f * deltaTime;
		}

		if (oldPos.y > ingredientsComponent->BounceStartPos.y)
		{
			transform2D->SetWorldPosition(ingredientsComponent->BounceStartPos);
			return Stationary;
		}

		transform2D->SetWorldPosition(oldPos);
	}

	return Bounce;
}

void Bouncing::Enter(powe::WorldEntity& worldEntity, IngredientsComponent* ingredientsComponent,
	powe::GameObjectID id)
{
	using namespace powe;
	ingredientsComponent->BouncingSpeed = ingredientsComponent->BouncingAccel;
	Transform2D* transform2D = worldEntity.GetComponent<Transform2D>(id);
	if (transform2D)
	{
		ingredientsComponent->BounceStartPos = transform2D->GetWorldPosition();
	}
}

void Bouncing::Exit(powe::WorldEntity&, IngredientsComponent* ingredientsComponent,
	powe::GameObjectID)
{
	ingredientsComponent->BounceTimeCounter = 0.0f;

	if (const auto gameObject = ingredientsComponent->Owner.lock())
	{
		gameObject->RemoveComponent<powe::SpriteComponent>();
//		gameObject->RemoveComponent<Rect2DCollider>();
//
//#ifdef _DEBUG
//
//		gameObject->RemoveComponent<powe::DebugRectangle>();
//
//#endif
	}
}
