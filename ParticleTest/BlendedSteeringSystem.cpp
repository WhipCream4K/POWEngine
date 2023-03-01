#include "BlendedSteeringSystem.h"

#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Random/Random.h"
#include "UserComponents.h"

using namespace powe;

BlendedSteeringSystem::BlendedSteeringSystem()
{
    DefineSystemKeys<powe::Transform2D,VelocityComponent,FleeComponent,WanderComponent>();
}

void BlendedSteeringSystem::OnUpdate(float deltaTime, powe::GameObjectID id [[maybe_unused]])
{
    const auto& [agentTransform,velocity,fleeComponent,wanderComponent] =
        GetComponentsView<Transform2D,VelocityComponent,FleeComponent,WanderComponent>();

    // const auto& agentTransform{m_Transform.at(id)};
    // const auto& velocity{m_Velocity.at(id)};
    // const auto& fleeComponent{m_FleeComponent.at(id)};
    // const auto& wanderComponent{m_WanderComponent.at(id)};

    // Flee Steering
    {
        const glm::fvec2 mousePos{GetWorld().GetInputSettings().GetMouseEnginePos()};
    
        // using mouse as flee target
        const auto& agentPos{agentTransform.GetPosition()};
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
            steering = (glm::normalize(agentPos - mousePos) * velocity.maxVelocity) 
            * fleeComponent.fleePower;
        }
        
    
        fleeComponent.steeringForce = steering;
    }
    
    // Wander Steering
    {
        const float halfJitter{m_MaxJitter / 2.0f};
        const glm::fvec2 randomJitterDir{
            Random::RandFloat(-halfJitter,halfJitter),
            Random::RandFloat(-halfJitter,halfJitter)
        };
    
        // const auto& [agentTransform,agentVelocity,wanderComp] =
        //     GetComponentsView<Transform2D,VelocityComponent,WanderComponent>();
    
        const glm::fvec2 wanderTarget{glm::normalize(randomJitterDir) * m_AimRadius};
    
        // calculate travel target
        const glm::fvec2 agentPos{agentTransform.GetPosition()};
        const glm::fvec2 aimOffset{glm::normalize(velocity.linearVelocity) * m_Offset};
        const glm::fvec2 target{agentPos + aimOffset + wanderTarget};
    
        const glm::fvec2 desiredVel{glm::normalize( target - agentPos) * velocity.maxVelocity};
        const glm::fvec2 steering{desiredVel};
    
        wanderComponent.steeringForce = steering;
    }
    
    
    glm::fvec2 blendSteering{};

    float totalWeight{};
    blendSteering += wanderComponent.steeringForce * wanderComponent.weightSteering;
    blendSteering += fleeComponent.steeringForce * fleeComponent.weightSteering;

    totalWeight = wanderComponent.weightSteering + fleeComponent.weightSteering;

    // scales
    if(totalWeight > 1.0f)
    {
        blendSteering *= 1.0f / totalWeight;
    }

    // Apply Force
    velocity.linearVelocity += (blendSteering - velocity.linearVelocity) * deltaTime;

    if(glm::length2(velocity.linearVelocity) >
        velocity.maxVelocity * velocity.maxVelocity)
    {
        velocity.linearVelocity = glm::normalize(velocity.linearVelocity) * velocity.maxVelocity;
    }
    
    agentTransform.SetWorldPosition(agentTransform.GetPosition() + velocity.linearVelocity * deltaTime);
}
