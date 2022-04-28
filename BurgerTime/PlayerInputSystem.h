#pragma once

#include "POWEngine/Core/ECS/SystemBase.h"

class PlayerInputSystem : public powe::SystemBase
{
public:

	PlayerInputSystem();
	
protected:

	void OnUpdate(float, powe::GameObjectID) override;
};

