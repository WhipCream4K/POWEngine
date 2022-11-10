#pragma once

#include "poweSystem.h"

class BlendedSteeringSystem : public powe::SystemBase
{
public:

    BlendedSteeringSystem();
    
protected:

    void OnCreate(powe::GameObjectID) override;
    void OnUpdate(float, powe::GameObjectID) override;

private:

    float m_MaxJitter{1.0f};
    float m_Offset{6.0f};
    float m_AimRadius{10.f};
    
    float m_EvadeTolerance{30.0f};
    float m_FleeTolerance{10.0f};
};
