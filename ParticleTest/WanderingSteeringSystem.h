#pragma once

#include "POWEngine/Math/Math.h"

#include "poweSystem.h"
#include "UserComponents.h"
// #include "POWEngine/Core/Components/Transform2D.h"

namespace powe
{
    class Transform2D;
}

class WanderingSteeringSystem : public powe::SystemBase
{
public:

    WanderingSteeringSystem();
    
protected:
    void OnCreate(powe::GameObjectID) override;
    void OnUpdate(float, powe::GameObjectID) override;
    
private:

    float m_MaxJitter{1.0f};
    float m_Offset{6.0f};
    float m_AimRadius{10.f};

    std::unordered_map<powe::GameObjectID,VelocityComponent*> m_Velocity{};
    // std::unordered_map<powe::GameObjectID,WanderComponent*> m_Wander{};
    std::unordered_map<powe::GameObjectID,powe::Transform2D*> m_AgentTransform{};
    // float m_AgentSpeed{150.0f};
};
