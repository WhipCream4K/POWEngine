#pragma once

#include "poweSystem.h"

class BoundAreaSystem : public powe::SystemBase
{
public:

    BoundAreaSystem();

    void OnUpdate(float, powe::GameObjectID) override;
    
private:
    
};
