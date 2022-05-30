#include "Player.h"

#include <powengine.h>

#include "BurgerTimeComponents.h"
#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Rendering/Components/Sprite/SpriteComponent.h"

SharedPtr<powe::GameObject> Player::Create(powe::WorldEntity& worldEntity)
{
	using namespace powe;

	const SharedPtr<GameObject> gameObject{ std::make_shared<GameObject>(worldEntity) };
	gameObject->AddComponent(Transform2D{ gameObject });
	gameObject->AddComponent(SpriteComponent{}, ComponentFlag::Sparse);
	gameObject->AddComponent(AnimationComponent{3,0.5f});

	PlayerSpeed* playerSpeed = gameObject->AddComponent(PlayerSpeed{});
	playerSpeed->speed = 150.0f;

	


	return gameObject;
}
