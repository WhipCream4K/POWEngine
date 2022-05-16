#include "PlayerInputSystem.h"


PlayerInputSystem::PlayerInputSystem()
	: SystemBase()
{
	DEFINE_SYSTEM_KEY(int, std::string);
}

void PlayerInputSystem::OnUpdate(float, powe::GameObjectID)
{
	const auto [num, shit] = GetComponentsView<int, std::string>();
}

