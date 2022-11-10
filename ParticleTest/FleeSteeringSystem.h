#pragma once

#include "POWEngine/ECS/SystemBase.h"

class FleeSteeringSystem : public powe::SystemBase
{
public:
    FleeSteeringSystem();

protected:

    void OnUpdate(float, powe::GameObjectID) override;
private:
    float m_EvadeTolerance{30.0f};
    float m_FleeTolerance{10.0f};
};
