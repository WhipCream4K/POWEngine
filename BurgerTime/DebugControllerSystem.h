#pragma once

#include <poweSystem.h>

class DebugControllerSystem : public powe::SystemBase
{
public:

	DebugControllerSystem();
protected:

	void OnUpdate(float, powe::GameObjectID) override;

private:

};

