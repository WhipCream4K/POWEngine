#pragma once

#include "POWEngine/Core/Components/BaseComponent.h"
#include "POWEngine/Math/Math.h"

struct SteeringComponent : powe::Component<SteeringComponent>
{
    glm::fvec2 linearVelocity{1.0f,1.0f};
    float angularVelocity{};
    float maxVelocity{};
};

struct DebugSteeringComponent : powe::Component<DebugSteeringComponent>
{
    glm::fvec4 boundArea{};
};
