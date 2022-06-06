#include "Ingredient.h"

#include "AudioManager.h"
#include "BurgerTimeComponents.h"
#include "POWEngine/Core/GameObject/GameObject.h"
#include "POWEngine/Core/Components/Transform2D.h"

#include "IngredientsComponent.h"
#include "IngredientState.h"
//#include "OnIngredientDropToPlatform.h"
#include "PlaySoundOnEvent.h"
#include "ScorableEvent.h"
#include "ScoreListener.h"

SharedPtr<powe::GameObject> Ingredient::Create(powe::WorldEntity& worldEntity, const SharedPtr<SceneFactory>& refScene, const IngredientsDesc& desc)
{
	using namespace powe;
	SharedPtr<GameObject> gameObject{ std::make_shared<GameObject>(worldEntity) };
	Transform2D* transform2D = gameObject->AddComponent(Transform2D{ gameObject });
	transform2D->SetWorldPosition(desc.position);


	IngredientsComponent* ingredientsComponent = gameObject->AddComponent(IngredientsComponent{});
	ingredientsComponent->CurrentState = IngredientState::Spawn;
	ingredientsComponent->MaxStepCount = 4;
	ingredientsComponent->SceneRef = refScene;
	ingredientsComponent->Type = desc.ingredientsType;
	ingredientsComponent->StartCol = desc.startCol;
	ingredientsComponent->StartRow = desc.startRow;
	ingredientsComponent->Owner = gameObject;
	ingredientsComponent->FallingSpeed = 100.0f;
	ingredientsComponent->BouncingStartVelocity = 100.0f;
	ingredientsComponent->BounceApexTime = 0.2f;
	ingredientsComponent->OnIngredientDropToPlatform = std::make_shared<ScorableEvent>();

	AudioManager* audioManager = worldEntity.FindUniqueComponent<AudioManager>();
	if(audioManager)
	{
		ingredientsComponent->OnIngredientDropToPlatform->Attach(audioManager->OnEventHappened.get());
	}

	DisplayManager* displayManager = worldEntity.FindUniqueComponent<DisplayManager>();
	if(displayManager)
	{
		ingredientsComponent->OnIngredientDropToPlatform->Attach(displayManager->ScoreListener.get());
	}

	return gameObject;

}
