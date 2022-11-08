#pragma once

#include "POWEngine/Math/Math.h"

#include "poweSystem.h"

class WanderingSteeringSystem : public powe::SystemBase
{
public:

    WanderingSteeringSystem();
protected:
    void OnUpdate(float, powe::GameObjectID) override;
    
private:

    float m_MaxJitter{1.0f};
    float m_Offset{6.0f};
    float m_AimRadius{4.0f};
    float m_AgentSpeed{100.0f};
    float m_MaxForce{0.1f};
};
