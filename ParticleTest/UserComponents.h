#pragma once

#include "POWEngine/Core/Components/BaseComponent.h"
#include "POWEngine/Math/Math.h"

struct VelocityComponent : powe::Component<VelocityComponent>
{
    glm::fvec2 linearVelocity{1.0f,1.0f};
    float maxVelocity{};
};

struct Steering
{
    glm::fvec2 steeringForce{};
    float weightSteering{};
};

struct WanderComponent : powe::Component<WanderComponent> , Steering
{
};

struct FleeComponent : powe::Component<FleeComponent> , Steering
{
    float fleeRadius{};
};

struct DebugSteeringComponent : powe::Component<DebugSteeringComponent>
{
    glm::fvec4 boundArea{};
    float agentSize{1.5f};
    bool isOptionsChanged{};
};
