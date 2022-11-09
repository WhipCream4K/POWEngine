#include "FleeSteeringSystem.h"

#include "UserComponents.h"
#include "POWEngine/Core/Components/Transform2D.h"


using namespace powe;

FleeSteeringSystem::FleeSteeringSystem()
{
    DefineSystemKeys<Transform2D,VelocityComponent>();
}

void FleeSteeringSystem::OnUpdate(float deltaTime, powe::GameObjectID)
{
    const auto& [transform,velocity] =
        GetComponentsView<Transform2D,VelocityComponent>();

    const glm::fvec2 mousePos{GetWorld()->GetInputSettings().GetMouseEngineAxisData()};
    
    // using mouse as flee target
    const auto& agentPos{transform->GetPosition()};
    const glm::fvec2 targetVec{agentPos - mousePos};
    
    if(glm::length2(targetVec) <= m_FleeTolerance)
    {
        const glm::fvec2 steering{(targetVec * velocity->maxVelocity) - velocity->linearVelocity};

        velocity->linearVelocity += steering * deltaTime;
        transform->SetWorldPosition(agentPos + velocity->linearVelocity * deltaTime);
    }
}
