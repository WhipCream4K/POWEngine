#pragma once

#include "POWEngine/Core/Components/InputComponent.h"

class TestCommand : public powe::AxisCommand
{
public:

	void Execute(powe::WorldEntity& worldEntity, float axisValue) override;
};

class FireCommand : public powe::ActionCommand
{
public:

	void Execute(powe::WorldEntity& worldEntity) override;
};

