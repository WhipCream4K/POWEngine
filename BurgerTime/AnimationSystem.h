#pragma once

#include <poweSystem.h>

class AnimationSystem : public powe::SystemBase
{
public:

	AnimationSystem();

protected:

	void OnUpdate(float, powe::GameObjectID) override;
};

