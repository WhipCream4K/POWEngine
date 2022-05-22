#include "PlayerInputSystem.h"

#include <powengine.h>
#include "BurgerTimeComponents.h"

PlayerInputSystem::PlayerInputSystem()
{
	DEFINE_SYSTEM_KEY(InputComponent);
}

void PlayerInputSystem::OnUpdate(float, powe::GameObjectID)
{
	POWLOGINFO("Hey");
	InputComponent* inputTest{ GetComponent<InputComponent>() };
	POWLOGINFO(std::to_string(inputTest->tag));
}
