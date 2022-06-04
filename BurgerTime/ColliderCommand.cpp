#include "ColliderCommand.h"

#include "POWEngine/Logger/LoggerUtils.h"
#include "BurgerTimeComponents.h"
#include "POWEngine/Core/WorldEntity/WorldEntity.h"
#include "POWEngine/Core/Components/Transform2D.h"
#include "IngredientsComponent.h"

void DebugTriggerEnter::OnEnter(powe::WorldEntity&, powe::GameObjectID owner, powe::GameObjectID other)
{
	std::stringstream enter{"This "};
	enter << owner << " Hit : " << other;
	POWLOGNORMAL(enter.str());
}

void OnStaticIngredientTrigger::OnEnter(powe::WorldEntity& worldEntity, powe::GameObjectID owner, powe::GameObjectID other)
{
	using namespace powe;

	// If is player
	if (worldEntity.GetComponent<PlayerTag>(other))
	{
		std::stringstream enter{ "This " };
		enter << owner << " Hit : " << other;
		POWLOGNORMAL(enter.str());

		CanWalkOnTile* canWalkOnTile{ worldEntity.GetComponent<CanWalkOnTile>(other) };
		if (canWalkOnTile)
		{
			const auto walkDir{ canWalkOnTile->movementDetails.currentMovementDir };
			if (walkDir == MoveDir::Left || walkDir == MoveDir::Right)
			{
				// 1. Drop down a little bit pixel
				Transform2D* transform2D = worldEntity.GetComponent<Transform2D>(owner);
				if (transform2D)
				{
					auto oldPos{ transform2D->GetWorldPosition() };
					oldPos.y += m_DropDownDistance;
					transform2D->SetWorldPosition(oldPos);
				}

				//2. Set a signal to parent that the player has hit a part of the ingredient
				StepHandler* stepHandler = worldEntity.GetComponent<StepHandler>(owner);
				if(stepHandler)
				{
					IngredientsComponent* ingredientsComponent = worldEntity.GetComponent<IngredientsComponent>(stepHandler->stepHandlerID);
					if(ingredientsComponent)
					{
						++ingredientsComponent->CurrentStepCount;
					}
				}
			}

		}
	}
}
