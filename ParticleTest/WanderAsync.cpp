#include "WanderAsync.h"

#include "UserComponents.h"
#include "POWEngine/Random/Random.h"


glm::fvec2 WanderAsync::CalculatePosition(glm::fvec4 boundArea,float deltaTime, powe::GameObjectID)
{
    // std::this_thread::sleep_for(std::chrono::milliseconds(2));

    const float halfJitter{m_MaxJitter / 2.0f};
    const glm::fvec2 randomJitterDir{
        ThreadSafeRandom::RandFloat(-halfJitter,halfJitter),
        ThreadSafeRandom::RandFloat(-halfJitter,halfJitter)
    };
    
    const auto& [agentTransform,agentVelocity,wanderComp] =
        GetComponentsView<PositionComponent,VelocityComponent,WanderComponent>();
    
    const glm::fvec2 wanderTarget{glm::normalize(randomJitterDir) * m_AimRadius};

    // const auto& agentTransform{GetComponent<Transform2D>()};
    // const auto& agentVelocity{GetComponent<VelocityComponent>()};
    
    // calculate travel target
    const glm::fvec2 agentPos{agentTransform.position};
    const glm::fvec2 aimOffset{glm::normalize(agentVelocity.linearVelocity) * m_Offset};
    const glm::fvec2 target{agentPos + aimOffset + wanderTarget};

    const glm::fvec2 desiredVel{glm::normalize( target - agentPos) * agentVelocity.maxVelocity};
    const glm::fvec2 steering{desiredVel};
    
    wanderComp.steeringForce = steering;

    agentVelocity.linearVelocity += wanderComp.steeringForce * deltaTime;

    if(glm::length2(agentVelocity.linearVelocity) >
        agentVelocity.maxVelocity * agentVelocity.maxVelocity)
    {
        agentVelocity.linearVelocity = glm::normalize(agentVelocity.linearVelocity) * agentVelocity.maxVelocity;
    }

    agentTransform.position += agentVelocity.linearVelocity * deltaTime;
    
    const float left{boundArea.x - boundArea.z * 0.5f};
    const float right{boundArea.x + boundArea.z * 0.5f};
    const float up{boundArea.y + boundArea.w * 0.5f};
    const float down{boundArea.y - boundArea.w * 0.5f};
    
    if (agentTransform.position.x < left - 2.0f)
    {
        agentTransform.position.x = right;
    }
    else if (agentTransform.position.x > right + 2.0f)
    {
        agentTransform.position.x = left;
    }
    
    if(agentTransform.position.y < down  - 2.0f)
    {
        agentTransform.position.y = up;
    }
    else if (agentTransform.position.y > up + 2.0f)
    {
        agentTransform.position.y = down;
    }
    
    // agentTransform.SetWorldPosition(agentTransform.GetPosition() + velocity.linearVelocity * deltaTime);
    
    SetTaskState(true);

    return agentTransform.position;
}
