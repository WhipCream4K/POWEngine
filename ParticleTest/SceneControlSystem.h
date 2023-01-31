#pragma once

#include "EngineStatsComponent.h"
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
    SceneControlSystem(const SharedPtr<powe::GameObject>& agentVertices);
    
protected:

    void OnPerGameObjectCreate(powe::GameObjectID) override;
    void OnUpdate(float, powe::GameObjectID) override;

private:

    // powe::SFML2DRectangle* m_DrawBoundingBox{};
    WeakPtr<powe::GameObject> m_AgentVertices{};
    AgentVerticesBatch* m_AgentVerticesBatch{};
    DebugSteeringComponent* m_DebugOpt{};
    EngineStatsComponent* m_EngineComponent{};
    SceneComponent* m_SceneComponent{};
    std::vector<powe::GameObjectID> m_StandByGameObjects{};
    int m_ActiveAgents{};
};
