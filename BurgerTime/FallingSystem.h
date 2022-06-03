#pragma once

#include <poweSystem.h>

class FallingSystem : public powe::SystemBase
{
public:

	FallingSystem(); 

protected:

	void OnUpdate(float, powe::GameObjectID) override;
};

