#pragma once

#include <poweSystem.h>

class PlayerInputSystem : public powe::SystemBase
{
public:

	PlayerInputSystem();
	
protected:

	void OnUpdate(float, powe::GameObjectID) override;
	void OnCreate(powe::GameObjectID) override;

private:

	float m_Timer{};
};

