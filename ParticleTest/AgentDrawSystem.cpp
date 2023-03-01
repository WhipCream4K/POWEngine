#include "AgentDrawSystem.h"

#include "UserComponents.h"
#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Core/GameObject/GameObject.h"
#include "POWEngine/Logger/LoggerUtils.h"
#include "POWEngine/Renderer/Renderer.h"

using namespace powe;


AgentDrawSystem::AgentDrawSystem()
{
    DefineSystemKeys<Transform2D,VelocityComponent>();
}

void AgentDrawSystem::OnCreate()
{
    auto& world{GetWorld()};
    if(!m_AgentVerticesBatch)
    {
        powe::GameObjectID agentBatchID{};
        m_AgentVerticesBatch = world.FindUniqueComponent<AgentVerticesBatch>(agentBatchID);
        
        DrawAsset* drawAsset{world.GetComponent<DrawAsset>(agentBatchID)};
        drawAsset->drawAsset = &m_AgentVerticesBatch->vertexBuffer;
    }

    if(!m_DebugOptions)
    {
        m_DebugOptions = world.FindUniqueComponent<DebugSteeringComponent>();
    }

    if(!m_AgentVerticesBatch || !m_DebugOptions)
    {
        throw std::runtime_error("AgentDrawSystem Initialization failed");
    }
}

void AgentDrawSystem::OnUpdate(float, powe::GameObjectID)
{
    auto& transform2D{GetComponent<Transform2D>()};
    
    const glm::vec2 centerPoint{ transform2D.GetPosition() };

    const sf::Vertex v1{sf::Vector2f{
        centerPoint.x - m_DebugOptions->agentSize * 0.5f,
        -centerPoint.y - m_DebugOptions->agentSize * 0.5f},
        sf::Color::White
    };

    const sf::Vertex v2{sf::Vector2f{
        centerPoint.x - m_DebugOptions->agentSize * 0.5f,
         -centerPoint.y + m_DebugOptions->agentSize * 0.5f},
        sf::Color::White
    };

    const sf::Vertex v3{sf::Vector2f{
        centerPoint.x + m_DebugOptions->agentSize * 0.5f,
        -centerPoint.y + m_DebugOptions->agentSize * 0.5f},
        sf::Color::White
    };

    const sf::Vertex v4{sf::Vector2f{
        centerPoint.x + m_DebugOptions->agentSize * 0.5f,
        -centerPoint.y - m_DebugOptions->agentSize * 0.5f},
        sf::Color::White
    };

    m_AgentVerticesBatch->vertexBuffer.append(v1);
    m_AgentVerticesBatch->vertexBuffer.append(v2);
    m_AgentVerticesBatch->vertexBuffer.append(v3);
    m_AgentVerticesBatch->vertexBuffer.append(v4);
}


