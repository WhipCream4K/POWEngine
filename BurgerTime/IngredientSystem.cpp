#include "IngredientSystem.h"

#include "IngredientsComponent.h"
#include "IngredientState.h"

IngredientSystem::IngredientSystem()
{
	DEFINE_SYSTEM_KEY(IngredientsComponent);
}

void IngredientSystem::OnUpdate(float deltaTime, powe::GameObjectID id)
{
	IngredientsComponent* ingredientsComponent = GetComponent<IngredientsComponent>();

	const auto oldState{ ingredientsComponent->CurrentState };
	if(oldState)
	{
		const auto newState{ oldState->Update(
			*GetWorld(),deltaTime,ingredientsComponent,id) };

		if(newState != oldState)
		{
			oldState->Exit(*GetWorld(), ingredientsComponent,id);
			newState->Enter(*GetWorld(), ingredientsComponent,id);

			ingredientsComponent->CurrentState = newState;
		}
	}
}
