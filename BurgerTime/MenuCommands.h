#pragma once

#include "POWEngine/Core/Components/InputComponent.h"


class MenuCommands : public powe::AxisCommand
{
public:

	void Execute(powe::WorldEntity& worldEntity, float deltaTime, powe::GameObjectID id, float axisValue) override;

private:

	int m_CurrentMovement{};
};

