#pragma once

#include "UserComponents.h"
#include "POWEngine/ECS/SystemBase.h"

namespace powe
{
    class SFML2DRectangle;
}

class SceneControlSystem : public powe::SystemBase
{
public:
    
    SceneControlSystem();
    
protected:

    void OnCreate(powe::GameObjectID) override;
    void OnUpdate(float, powe::GameObjectID) override;

private:

    powe::SFML2DRectangle* m_DrawBoundingBox{};
    DebugSteeringComponent* m_DebugOpt{};
    SceneComponent* m_SceneComponent{};
    std::vector<powe::GameObjectID> m_StandByGameObjects{};
    int m_ActiveAgents{};
};
