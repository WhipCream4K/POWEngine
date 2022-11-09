#pragma once

#include "UserComponents.h"
#include "POWEngine/ECS/SystemBase.h"

namespace powe
{
    class SFML2DRectangle;
}

class DebugSteeringSystem : public powe::SystemBase
{
public:
    
    DebugSteeringSystem();
    
protected:

    void OnCreate(powe::GameObjectID) override;
    void OnUpdate(float, powe::GameObjectID) override;

private:

    powe::SFML2DRectangle* m_DrawBoundingBox{};
    DebugSteeringComponent* m_DebugOpt{};
};
