#include "Ingredient.h"

#include "POWEngine/Core/GameObject/GameObject.h"
#include "POWEngine/Core/Components/Transform2D.h"

#include "IngredientsComponent.h"
#include "IngredientState.h"

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

	return gameObject;

}
