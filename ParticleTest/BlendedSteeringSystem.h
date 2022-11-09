#pragma once

#include "poweSystem.h"

class BlendedSteeringSystem : public powe::SystemBase
{
public:

    BlendedSteeringSystem();
    
protected:

    void OnCreate(powe::GameObjectID) override;
    void OnUpdate(float, powe::GameObjectID) override;
    
};
