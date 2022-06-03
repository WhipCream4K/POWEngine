#include "DelayedMovementSystem.h"

#include "POWEngine/Core/Components/Transform2D.h"

#include "BurgerTimeComponents.h"

DelayedMovementSystem::DelayedMovementSystem()
{
	using namespace powe;

	DEFINE_SYSTEM_KEY(Transform2D, DelayedMovement,CanWalkOnTile,Speed);
}

void DelayedMovementSystem::OnUpdate(float deltaTime, powe::GameObjectID)
{
	using namespace powe;

	const auto& [transform2D, delayMovement, canWalkOnTile,speed] = GetComponentsView<Transform2D, DelayedMovement, CanWalkOnTile,Speed>();

	if(delayMovement->isMoving)
	{
		if(delayMovement->timeCounter < delayMovement->timeToReachNextSplit)
		{
			delayMovement->timeCounter += deltaTime;
			auto oldPos{ transform2D->GetWorldPosition() };
			oldPos.x += speed->speed * deltaTime;
			transform2D->SetWorldPosition(oldPos);
		}
		else
		{
			delayMovement->isMoving = false;
			delayMovement->timeCounter -= delayMovement->timeToReachNextSplit;
		}

		//canWalkOnTile->movementDetails.futurePos = (delayMovement->timeToReachNextSplit * oldPos);
	}
}
