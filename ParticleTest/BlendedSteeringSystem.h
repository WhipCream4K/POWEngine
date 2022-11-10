#pragma once

#include "poweSystem.h"

namespace powe
{
    class Transform2D;
}

struct VelocityComponent;
struct FleeComponent;
struct WanderComponent;
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

    std::unordered_map<powe::GameObjectID,powe::Transform2D*> m_Transform{}; 
    std::unordered_map<powe::GameObjectID,VelocityComponent*> m_Velocity{};
    std::unordered_map<powe::GameObjectID,FleeComponent*> m_FleeComponent{};
    std::unordered_map<powe::GameObjectID,WanderComponent*> m_WanderComponent{};
};
