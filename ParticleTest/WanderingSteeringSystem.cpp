#include "WanderingSteeringSystem.h"

#include <imgui.h>

#include "UserComponents.h"
#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Random/Random.h"

using namespace powe;

WanderingSteeringSystem::WanderingSteeringSystem()
{
    DefineSystemKeys<powe::Transform2D,VelocityComponent>();
}

void WanderingSteeringSystem::OnCreate(powe::GameObjectID id)
{
    m_AgentTransform[id] = GetComponent<Transform2D>();
    m_Velocity[id] = GetComponent<VelocityComponent>();
    // m_Wander[game_object_id] = GetComponent<WanderComponent>();
}

void WanderingSteeringSystem::OnUpdate(float deltaTime, powe::GameObjectID id [[maybe_unused]])
{
    const float halfJitter{m_MaxJitter / 2.0f};
    const glm::fvec2 randomJitterDir{
        Random::RandFloat(-halfJitter,halfJitter),
        Random::RandFloat(-halfJitter,halfJitter)
    };
    
    const auto& [agentTransform,agentVelocity] = GetComponentsView<Transform2D,VelocityComponent>();
    
    const glm::fvec2 wanderTarget{glm::normalize(randomJitterDir) * m_AimRadius};

    // const auto& agentTransform{GetComponent<Transform2D>()};
    // const auto& agentVelocity{GetComponent<VelocityComponent>()};
    
    
    // calculate travel target
    const glm::fvec2 agentPos{agentTransform->GetPosition()};
    const glm::fvec2 aimOffset{glm::normalize(agentVelocity->linearVelocity) * m_Offset};
    const glm::fvec2 target{agentPos + aimOffset + wanderTarget};

    const glm::fvec2 desiredVel{glm::normalize( target - agentPos) * agentVelocity->maxVelocity};
    const glm::fvec2 steering{(desiredVel - agentVelocity->linearVelocity)};
    
    // Apply force
    agentVelocity->linearVelocity += steering * deltaTime;
    agentTransform->SetWorldPosition(agentPos + (agentVelocity->linearVelocity * deltaTime));
    
}
