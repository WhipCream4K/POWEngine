#include "WanderingSteeringSystem.h"

#include <imgui.h>
#include <imgui-SFML.h>

#include "UserComponents.h"
#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Random/Random.h"

using namespace powe;

WanderingSteeringSystem::WanderingSteeringSystem()
{
    DefineSystemKeys<powe::Transform2D,SteeringComponent>();
}

void WanderingSteeringSystem::OnUpdate(float deltaTime, powe::GameObjectID)
{
    const float halfJitter{m_MaxJitter / 2.0f};
    const glm::fvec2 randomJitterDir{
        Random::RandFloat(-halfJitter,halfJitter),
        Random::RandFloat(-halfJitter,halfJitter)
    };
    
    const auto& [agentTransform,agentSteering] = GetComponentsView<Transform2D,SteeringComponent>();
    
    const glm::fvec2 wanderTarget{glm::normalize(randomJitterDir) * m_AimRadius};
    
    // calculate travel target
    const glm::fvec2 agentPos{agentTransform->GetPosition()};
    const glm::fvec2 aimOffset{glm::normalize(agentSteering->linearVelocity) * m_Offset};
    const glm::fvec2 target{agentPos + aimOffset + wanderTarget};

    const glm::fvec2 desiredVel{glm::normalize(agentPos - target) * m_AgentSpeed};
    glm::fvec2 steering{(desiredVel - agentSteering->linearVelocity)};

    if(glm::length(steering) > m_MaxForce)
    {
        steering = glm::normalize(steering) * m_MaxForce; 
    }

    agentSteering->linearVelocity = (steering * deltaTime) + agentSteering->linearVelocity;
    
    agentTransform->SetWorldPosition(agentPos + (agentSteering->linearVelocity * deltaTime));

    
}
