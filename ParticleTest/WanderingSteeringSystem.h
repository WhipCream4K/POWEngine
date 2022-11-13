#pragma once

#include "POWEngine/ECS/SystemBase.h"

namespace powe
{
    class Transform2D;
}

class WanderingSteeringSystem : public powe::SystemBase
{
public:

    WanderingSteeringSystem();
    
protected:
    
    void OnUpdate(float, powe::GameObjectID) override;
    
private:

    float m_MaxJitter{1.0f};
    float m_Offset{6.0f};
    float m_AimRadius{10.f};
};
