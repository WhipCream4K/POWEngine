﻿#pragma once

#include "POWEngine/ECS/SystemBase.h"

class FleeSteeringSystem : public powe::SystemBase
{
public:
    FleeSteeringSystem();

protected:

    void OnUpdate(float, powe::GameObjectID) override;
private:
    float m_FleeTolerance{16.0f};
};
