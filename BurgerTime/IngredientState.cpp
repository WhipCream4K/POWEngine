#include "IngredientState.h"

#include "BurgerTimeComponents.h"
#include "IngredientsComponent.h"
#include "IngredientsStatic.h"
#include "SceneFactory.h"
#include "POWEngine/Core/WorldEntity/WorldEntity.h"
#include "POWEngine/Core/GameObject/GameObject.h"
#include "POWEngine/Core/Components/Transform2D.h"

SharedPtr<IngredientState> IngredientState::Stationary{ std::make_shared<StationaryIngredient>() };
SharedPtr<IngredientState> IngredientState::Falling{ std::make_shared<FallingIngredient>() };
SharedPtr<IngredientState> IngredientState::Spawn{ std::make_shared<SpawnIngredient>() };

SharedPtr<IngredientState> SpawnIngredient::Update(powe::WorldEntity& , float ,
	IngredientsComponent* , powe::GameObjectID )
{
	return Stationary;
}

SharedPtr<IngredientState> StationaryIngredient::Update(powe::WorldEntity&,float,
														IngredientsComponent* ingredientsComponent, powe::GameObjectID)
{
	if(ingredientsComponent->CurrentStepCount >= ingredientsComponent->MaxStepCount)
	{
		return Falling;
	}

	return Stationary;
}

void StationaryIngredient::Enter(powe::WorldEntity& worldEntity, IngredientsComponent* ingredientsComponent, powe::GameObjectID owner)
{
	using namespace powe;

	ingredientsComponent->CurrentStepCount = 0;

	GameObjectID sceneDataID{};
	DynamicSceneData* dynamicSceneData = worldEntity.FindUniqueComponent<DynamicSceneData>(sceneDataID);
	if(!dynamicSceneData)
		return;
	Transform2D* transform2D = worldEntity.GetComponent<Transform2D>(owner);
	if (!transform2D)
		return;

	// Initialize colliders
	const IngredientsDesc desc{sceneDataID,ingredientsComponent->Type,transform2D->GetWorldPosition(),0,0,owner};
	auto colldiers{ IngredientsStatic::Create(worldEntity,desc) };

	ingredientsComponent->StepColliders.insert(ingredientsComponent->StepColliders.begin(), colldiers.begin(), colldiers.end());

	if(const auto scene = ingredientsComponent->SceneRef.lock())
	{
		for (const auto& gb : colldiers)
		{
			scene->AddGameObject(gb);
		}
	}
}

void StationaryIngredient::Exit(powe::WorldEntity& worldEntity, IngredientsComponent* ingredientsComponent, powe::GameObjectID)
{
	for (auto & weakGameObject : ingredientsComponent->StepColliders)
	{
		if(const auto collider{weakGameObject.lock()})
		{
			worldEntity.RemoveGameObject(collider->GetID());

			if(const auto scene{ingredientsComponent->SceneRef.lock()})
			{
				scene->RemoveGameObject(collider);
			}
		}
	}
}

SharedPtr<IngredientState> FallingIngredient::Update(powe::WorldEntity& , float ,
	IngredientsComponent* , powe::GameObjectID )
{
	return Falling;
}

void FallingIngredient::Enter(powe::WorldEntity& worldEntity, IngredientsComponent* ingredientsComponent,
							  powe::GameObjectID gameObjectId)
{
	ingredientsComponent;
	worldEntity.AddComponentToGameObject(gameObjectId, FallingComponent{ 100.0f });
}

void FallingIngredient::Exit(powe::WorldEntity& worldEntity, IngredientsComponent* ingredientsComponent,
	powe::GameObjectID gameObjectId)
{
	worldEntity;
	gameObjectId;
	ingredientsComponent;
}
