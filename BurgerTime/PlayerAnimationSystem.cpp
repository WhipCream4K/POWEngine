#include "PlayerAnimationSystem.h"
#include "BurgerTimeComponents.h"

PlayerAnimationSystem::PlayerAnimationSystem()
{
	DEFINE_SYSTEM_KEY(AnimationComponent, CanWalkOnTile, PlayerTag);
}

void PlayerAnimationSystem::OnUpdate(float, powe::GameObjectID)
{
	const auto& [animationComp, canWalkOnTile] = GetComponentsView<AnimationComponent, CanWalkOnTile>();

	switch (canWalkOnTile->movementDetails.currentMovementDir)
	{
	case MoveDir::None: 



		break;
	case MoveDir::Left: 



		break;
	case MoveDir::Right: 


		break;
	case MoveDir::Up:


		break;
	case MoveDir::Down: 



		break;
	default: ;
	}

}
