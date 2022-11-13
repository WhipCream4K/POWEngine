#pragma once
#include "POWEngine/ECS/ThreadExecute.h"

class WanderAsync : public powe::ThreadExecute
{
public:
    
    WanderAsync() = default;
    
    glm::fvec2 CalculatePosition(glm::fvec4 boundArea,float deltaTime,powe::GameObjectID);
    
private:

    // glm::fvec4 m_BoundArea{};
    float m_MaxJitter{1.0f};
    float m_Offset{6.0f};
    float m_AimRadius{10.f};
};
