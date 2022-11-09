#include "FleeSteeringSystem.h"

#include "UserComponents.h"
#include "POWEngine/Core/Components/Transform2D.h"


using namespace powe;

FleeSteeringSystem::FleeSteeringSystem()
{
    DefineSystemKeys<Transform2D,VelocityComponent,FleeComponent>();
}

void FleeSteeringSystem::OnUpdate(float, powe::GameObjectID)
{
    const auto& [transform,velocity,fleeSteering] =
        GetComponentsView<Transform2D,VelocityComponent,FleeComponent>();

    const glm::fvec2 mousePos{GetWorld()->GetInputSettings().GetMouseEngineAxisData()};
    
    // using mouse as flee target
    const auto& agentPos{transform->GetPosition()};
    const glm::fvec2 distanceFromTarget{mousePos - agentPos};
    
}
