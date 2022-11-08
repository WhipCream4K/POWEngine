#pragma once

#include <POWEngine/Math/Math.h>

#include "poweSystem.h"

class BoundAreaSystem : public powe::SystemBase
{
public:

    BoundAreaSystem(const glm::fvec4& box);
        
    void OnUpdate(float, powe::GameObjectID) override;
    
private:

    glm::fvec4 m_BoundingBox{};
};
