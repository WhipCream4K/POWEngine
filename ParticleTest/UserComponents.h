#pragma once

#include "POWEngine/Core/Components/BaseComponent.h"
#include "POWEngine/Math/Math.h"

struct SteeringComponent : powe::Component<SteeringComponent>
{
    glm::fvec2 linearVelocity{};
    float angularVelocity{};
};
