#pragma once

#include "POWEngine/Core/Components/InputComponent.h"

class HorizontalMovement : public powe::AxisCommand
{
public:

	void Execute(powe::WorldEntity& worldEntity, float deltaTime, powe::GameObjectID id, float axisValue) override;
};

class VerticalMovement : public powe::AxisCommand
{
public:

	void Execute(powe::WorldEntity& worldEntity, float deltaTime, powe::GameObjectID id, float axisValue) override;
};

class DebugHorizontalMovement : public powe::AxisCommand
{
public:

	void Execute(powe::WorldEntity& worldEntity, float deltaTime, powe::GameObjectID id, float axisValue) override;
};

class ThrowPepper : public powe::ActionCommand
{
public:

	void Execute(powe::WorldEntity& worldEntity, float deltaTime, powe::GameObjectID id, InputEvent inputEvent) override;
};

class DebugVerticalMovement : public powe::AxisCommand
{
public:

	void Execute(powe::WorldEntity& worldEntity, float deltaTime, powe::GameObjectID id, float axisValue) override;
};
