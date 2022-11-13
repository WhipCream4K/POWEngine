#include "FleeSteeringSystem.h"

#include "UserComponents.h"
#include "POWEngine/Core/Components/Transform2D.h"


using namespace powe;

FleeSteeringSystem::FleeSteeringSystem()
{
    DefineSystemKeys<Transform2D, VelocityComponent, FleeComponent>();
}

void FleeSteeringSystem::OnUpdate(float, powe::GameObjectID)
{
    const auto& [transform,velocity,fleeComp] =
        GetComponentsView<Transform2D, VelocityComponent, FleeComponent>();

    const glm::fvec2 mousePos{GetWorld().GetInputSettings().GetMouseEnginePos()};

    // using mouse as flee target
    const auto& agentPos{transform.GetPosition()};
    const glm::fvec2 targetVec{mousePos - agentPos};

    const float distToTarget{glm::length(targetVec)};
    
    const float dynamicFactor{distToTarget / velocity.maxVelocity};
    const glm::fvec2 futurePos{mousePos + velocity.linearVelocity * dynamicFactor};

    glm::fvec2 steering{};
    if (glm::distance2(futurePos, agentPos) <= m_EvadeTolerance * m_EvadeTolerance)
    {
        steering =
            (glm::normalize(agentPos - futurePos) * velocity.maxVelocity);
    }
    else if (distToTarget <= m_FleeTolerance)
    {
        steering = (glm::normalize(agentPos - mousePos) * velocity.maxVelocity) * fleeComp.fleePower;
    }

    fleeComp.steeringForce = steering;
}
