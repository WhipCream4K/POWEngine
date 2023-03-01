#pragma once


#include "POWEngine/Renderer/SFML/SFML2DRenderer.h"
#include "POWEngine/Renderer/SFML/SFML2DRendererThreaded.h"
#include "POWEngine/Renderer/System/RenderSystemBase.h"


struct AgentVerticesBatch;
struct DebugSteeringComponent;

namespace powe
{
    class GameObject;
}

class AgentDrawSystem : public powe::SystemBase
{
public:

    AgentDrawSystem();

protected:

    void OnCreate() override;
    void OnUpdate(float, powe::GameObjectID) override;

private:
    
    AgentVerticesBatch* m_AgentVerticesBatch{};
    DebugSteeringComponent* m_DebugOptions{};
};
