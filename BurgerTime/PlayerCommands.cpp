#include "PlayerCommands.h"

#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Core/WorldEntity/WorldEntity.h"

#include "BurgerTimeComponents.h"

void HorizontalMovement::Execute(
	powe::WorldEntity& worldEntity, 
	float deltaTime, 
	powe::GameObjectID id,
	float axisValue)
{
	using namespace powe;

	Transform2D* transform2D{ worldEntity.GetComponent<Transform2D>(id) };
	PlayerSpeed* playerSpeed{ worldEntity.GetComponent<PlayerSpeed>(id) };

	if (!transform2D || !playerSpeed)
		return;

	glm::fvec2 horizontal{ transform2D->GetWorldPosition() };
	horizontal.x += axisValue * deltaTime * playerSpeed->speed;
	transform2D->SetWorldPosition(horizontal);
}

void VerticalMovement::Execute(
	powe::WorldEntity& worldEntity,
	float deltaTime,
	powe::GameObjectID id, float axisValue)
{
	using namespace powe;

	Transform2D* transform2D{ worldEntity.GetComponent<Transform2D>(id) };
	PlayerSpeed* playerSpeed{ worldEntity.GetComponent<PlayerSpeed>(id) };

	if (!transform2D || !playerSpeed)
		return;

	glm::fvec2 horizontal{ transform2D->GetWorldPosition() };
	horizontal.y += axisValue * deltaTime * playerSpeed->speed;
	transform2D->SetWorldPosition(horizontal);
}
