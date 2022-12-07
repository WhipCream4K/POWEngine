#pragma once

#include "poweSystem.h"
// #include "UserComponents.h"

namespace powe
{
    class Transform2D;
}

struct CellComponent;
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

    void UpdateCellRendering(powe::Transform2D& transform,CellComponent& cellComponent);
    
    float m_MaxJitter{1.0f};
    float m_Offset{6.0f};
    float m_AimRadius{10.f};
    
    float m_EvadeTolerance{30.0f};
    float m_FleeTolerance{10.0f};
};
