#pragma once

#include <poweSystem.h>

class PlayerAnimationSystem : public powe::SystemBase
{
public:

	PlayerAnimationSystem();

protected:

	void OnUpdate(float, powe::GameObjectID) override;

	//std::unordered_map<int, glm::fvec4> m_PlayerSprite;
};

