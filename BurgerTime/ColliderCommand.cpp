#include "ColliderCommand.h"

#include "POWEngine/Logger/LoggerUtils.h"
#include "BurgerTimeComponents.h"
#include "POWEngine/Core/WorldEntity/WorldEntity.h"
#include "POWEngine/Core/Components/Transform2D.h"
#include "IngredientsComponent.h"
#include "IngredientState.h"
#include "Rect2DCollider.h"

void DebugTriggerEnter::OnEnter(powe::WorldEntity&, Rect2DCollider* , Rect2DCollider*,
	powe::GameObjectID owner, powe::GameObjectID other)
{
	std::stringstream enter{ "This " };
	enter << owner << " Hit : " << other;
	POWLOGNORMAL(enter.str());
}

void OnStaticIngredientTrigger::OnEnter(powe::WorldEntity& worldEntity, Rect2DCollider* ,
	Rect2DCollider*, powe::GameObjectID owner, powe::GameObjectID other)
{
	using namespace powe;

	// If is player
	if (worldEntity.GetComponent<PlayerTag>(other))
	{

#ifdef _DEBUG
		std::stringstream enter{ "This " };
		enter << owner << " Hit : " << other;
		POWLOGNORMAL(enter.str());
#endif

		CanWalkOnTile* canWalkOnTile{ worldEntity.GetComponent<CanWalkOnTile>(other) };
		if (canWalkOnTile)
		{
			const auto walkDir{ canWalkOnTile->movementDetails.currentMovementDir };
			if (walkDir == MoveDir::Left || walkDir == MoveDir::Right)
			{
				StepHandler* stepHandler = worldEntity.GetComponent<StepHandler>(owner);
				if (!stepHandler)
					return;

				IngredientsComponent* ingredientsComponent = worldEntity.GetComponent<IngredientsComponent>(stepHandler->stepHandlerID);
				if (!ingredientsComponent)
					return;

				//2. Set a signal to parent that the player has hit a part of the ingredient
				if (!stepHandler->hasAlreadySteppedOn)
				{
					// 1. Drop down a little bit pixel
					Transform2D* transform2D = worldEntity.GetComponent<Transform2D>(owner);
					if (transform2D)
					{
						auto oldPos{ transform2D->GetWorldPosition() };
						oldPos.y += ingredientsComponent->StepPushDistance;
						transform2D->SetWorldPosition(oldPos);
						ingredientsComponent->StepPushDistance -= ingredientsComponent->StepPushDecreaseOffset;
					}

					stepHandler->hasAlreadySteppedOn = true;
					++ingredientsComponent->CurrentStepCount;

				}
			}

		}
	}

	// Other Ingredients Hits
	IngredientsComponent* ingredientsComponent = worldEntity.GetComponent<IngredientsComponent>(other);
	if (ingredientsComponent)
	{
		StepHandler* stepHandler = worldEntity.GetComponent<StepHandler>(owner);
		if (stepHandler)
		{
			ingredientsComponent = worldEntity.GetComponent<IngredientsComponent>(stepHandler->stepHandlerID);
			ingredientsComponent->CurrentStepCount = ingredientsComponent->MaxStepCount;
		}
	}
}

//void PlateTriggerEnter::OnEnter(powe::WorldEntity& worldEntity, Rect2DCollider* ownerCollider, Rect2DCollider* otherCollider,
//	powe::GameObjectID owner, powe::GameObjectID other)
//{
//	using namespace powe;
//	IngredientsComponent* ingredientsComponent = worldEntity.GetComponent<IngredientsComponent>(other);
//	if (!ingredientsComponent)
//		return;
//
//	// 1. Catch an ingredient falling
//	// 2. Move the collider up of its onw extent
//	// 3. Play sound?
//	// 4. if all ingredients has formed on this plate the notify
//	Transform2D* transform2D{ worldEntity.GetComponent<Transform2D>(other) };
//	if (!transform2D)
//		return;
//
//
//}

void OnFallingIngredientTrigger::OnEnter(powe::WorldEntity& worldEntity, Rect2DCollider*,
	Rect2DCollider* otherCollider, powe::GameObjectID owner, powe::GameObjectID other)
{
	using namespace powe;

	PlateComponent* plateComponent = worldEntity.GetComponent<PlateComponent>(other);
	if (plateComponent)
	{
		IngredientsComponent* ingredientsComponent = worldEntity.GetComponent<IngredientsComponent>(owner);
		if (!ingredientsComponent)
			return;

		++plateComponent->ingredientStackCount;
		if (plateComponent->ingredientStackCount >= plateComponent->maxStackCount)
		{
			// Do notify scene maybe
		}

		Transform2D* colliderTransform{ worldEntity.GetComponent<Transform2D>(other) };
		if(colliderTransform)
		{
			auto oldPos{ colliderTransform->GetWorldPosition() };

			ingredientsComponent->BounceStartPos = oldPos;
			ingredientsComponent->IsOnPlate = true;

			oldPos.y -= otherCollider->Size.y + plateComponent->realIngredientSize.y;
			colliderTransform->SetWorldPosition(oldPos);
		}
	}
}
