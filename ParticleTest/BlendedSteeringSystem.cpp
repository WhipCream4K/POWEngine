#include "BlendedSteeringSystem.h"

#include "UserComponents.h"
#include "POWEngine/Core/Components/Transform2D.h"

using namespace powe;

BlendedSteeringSystem::BlendedSteeringSystem()
{
    DefineSystemKeys<powe::Transform2D,VelocityComponent,FleeComponent,WanderComponent>();
}

void BlendedSteeringSystem::OnCreate(powe::GameObjectID)
{
    
}

void BlendedSteeringSystem::OnUpdate(float, powe::GameObjectID)
{
    // auto& [agentTransform,velocity,fleeComponent,wanderComponent] =
    //     GetComponentsView<Transform2D,VelocityComponent,FleeComponent,WanderComponent>();

    
}
