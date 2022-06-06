#include "PlayerCommands.h"

#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Core/WorldEntity/WorldEntity.h"
#include "OnPlayerThrowPepper.h"
#include "BurgerTimeComponents.h"

void HorizontalMovement::Execute(
	powe::WorldEntity& worldEntity, 
	float deltaTime, 
	powe::GameObjectID id,
	float axisValue)
{
	using namespace powe;

	Transform2D* transform2D{ worldEntity.GetComponent<Transform2D>(id) };
	CanWalkOnTile* canWalkOnTile{ worldEntity.GetComponent<CanWalkOnTile>(id) };
	const Speed* playerSpeed{ worldEntity.GetComponent<Speed>(id) };
	//DelayedMovement* delayedMovement{ worldEntity.GetComponent<DelayedMovement>(id) };

	if (!transform2D || !canWalkOnTile || !playerSpeed)
		return;

	glm::fvec2 horizontal{ transform2D->GetWorldPosition() };
	horizontal.x += axisValue * deltaTime * playerSpeed->speed;

	canWalkOnTile->movementDetails.currentMovementDir = axisValue > 0.0f ? MoveDir::Right : MoveDir::Left;
	//delayedMovement->isMoving = true;
	canWalkOnTile->movementDetails.futurePos = horizontal;


	//transform2D->SetWorldPosition(horizontal);
}

void VerticalMovement::Execute(
	powe::WorldEntity& worldEntity,
	float deltaTime,
	powe::GameObjectID id, float axisValue)
{
	using namespace powe;

	Transform2D* transform2D{ worldEntity.GetComponent<Transform2D>(id) };
	CanWalkOnTile* canWalkOnTile{ worldEntity.GetComponent<CanWalkOnTile>(id) };
	const Speed* playerSpeed{ worldEntity.GetComponent<Speed>(id) };

	if (!transform2D || !canWalkOnTile || !playerSpeed)
		return;

	glm::fvec2 currentPos{ transform2D->GetWorldPosition() };
	currentPos.y += axisValue * deltaTime * playerSpeed->climbSpeed;

	canWalkOnTile->movementDetails.currentMovementDir = axisValue > 0.0f ? MoveDir::Down : MoveDir::Up;
	canWalkOnTile->movementDetails.futurePos = currentPos;
}

void DebugHorizontalMovement::Execute(powe::WorldEntity& worldEntity, float deltaTime, powe::GameObjectID id,
	float axisValue)
{
	using namespace powe;

	Transform2D* transform2D{ worldEntity.GetComponent<Transform2D>(id) };
	const Speed* playerSpeed{ worldEntity.GetComponent<Speed>(id) };
	//DelayedMovement* delayedMovement{ worldEntity.GetComponent<DelayedMovement>(id) };

	if (!transform2D || !playerSpeed)
		return;

	glm::fvec2 horizontal{ transform2D->GetWorldPosition() };
	horizontal.x += axisValue * deltaTime * playerSpeed->speed;

	transform2D->SetWorldPosition(horizontal);
}

void ThrowPepper::Execute(powe::WorldEntity& worldEntity, float, powe::GameObjectID id, InputEvent inputEvent)
{
	if(inputEvent == InputEvent::IE_Pressed)
	{
		PlayerTag* playerTag = worldEntity.GetComponent<PlayerTag>(id);
		if(playerTag)
		{
			if(playerTag->OnPlayerThrowPepper)
			{
				playerTag->OnPlayerThrowPepper->SignalThrowPepper(worldEntity,playerTag->playerIndex);
			}
		}
	}
}

void DebugVerticalMovement::Execute(powe::WorldEntity& worldEntity, float deltaTime, powe::GameObjectID id,
									float axisValue)
{
	using namespace powe;

	Transform2D* transform2D{ worldEntity.GetComponent<Transform2D>(id) };
	const Speed* playerSpeed{ worldEntity.GetComponent<Speed>(id) };

	if (!transform2D || !playerSpeed)
		return;

	glm::fvec2 vertical{ transform2D->GetWorldPosition() };
	vertical.y += axisValue * deltaTime * playerSpeed->speed;

	transform2D->SetWorldPosition(vertical);
}
