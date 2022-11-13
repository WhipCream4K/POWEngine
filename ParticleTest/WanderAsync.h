#pragma once
#include "POWEngine/ECS/ThreadExecute.h"

class WanderAsync : public powe::ThreadExecute
{
public:
    
    WanderAsync() = default;
    
    glm::fvec2 CalculatePosition(float deltaTime,powe::GameObjectID);
};
