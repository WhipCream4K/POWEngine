#include "SceneControlSystem.h"

#include "UserComponents.h"
#include "POWEngine/Core/GameObject/GameObject.h"
#include "POWEngine/Logger/Logger.h"
#include "POWEngine/Logger/LoggerUtils.h"
#include "POWEngine/Random/Random.h"
// #include "POWEngine/Renderer/Components/Debug2D/SFML/SFML2DShapeComponent.h"

#include "POWEngine/Debug/imgui/ImGUI.h"


using namespace powe;

SceneControlSystem::SceneControlSystem()
{
    DefineSystemKeys<SceneComponent, DebugSteeringComponent>();

    m_FrameTimePoints = 100;
    m_FrameTimes.resize(m_FrameTimePoints);
}

SceneControlSystem::SceneControlSystem(const SharedPtr<powe::GameObject>& agentVertices)
    : m_AgentVertices(agentVertices)
{
    DefineSystemKeys<SceneComponent, DebugSteeringComponent>();
}

void SceneControlSystem::OnPerGameObjectCreate(powe::GameObjectID id)
{
    const auto& world{GetWorld()};

    if (!m_DebugOpt)
    {
        m_DebugOpt = world.GetComponent<DebugSteeringComponent>(id);
        m_ActiveAgents = m_DebugOpt->activeAgents;
    }

    if (!m_SceneComponent)
        m_SceneComponent = world.GetComponent<SceneComponent>(id);

    if(!m_EngineComponent)
        m_EngineComponent = world.GetComponent<EngineStatsComponent>(id);

#ifndef EntityDraw
    if(!m_AgentVerticesBatch)
    {
        m_AgentVerticesBatch = world.FindUniqueComponent<AgentVerticesBatch>();
    }
#endif
    
    if (!m_SceneComponent || !m_DebugOpt || !m_EngineComponent)
        POWLOGERROR("No Scene Control Data");
}

void SceneControlSystem::OnUpdate(float deltaTime [[maybe_unused]], powe::GameObjectID)
{

    ++m_EngineComponent->fpsCounter;
    m_EngineComponent->fpsTimeStamp += deltaTime;
    
    if(m_EngineComponent->fpsTimeStamp >= 1.0f)
    {
        m_EngineComponent->fpsTimeStamp -= 1.0f;
        m_EngineComponent->fps = m_EngineComponent->fpsCounter;
        m_EngineComponent->fpsCounter = 0;

    }

    // reset batch count
#ifndef EntityDraw
    m_AgentVerticesBatch->vertexID = 0;
    m_AgentVerticesBatch->vertexBuffer.clear();
#endif

#if USE_IMGUI
    
    ImGui::Begin("Debug Window");

    m_FrameTimes[0] = deltaTime;

    ImGui::PlotLines("Frame Times",m_FrameTimes.data(),m_FrameTimePoints,0,
        std::to_string(deltaTime * 1000.0f).append(" ms").c_str());

    for (int i = m_FrameTimePoints - 2; i >= 0; --i)
    {
        m_FrameTimes[i + 1] = m_FrameTimes[i];
    }
    
    ImGui::DragInt("FPS",&m_EngineComponent->fps);
    
    if (ImGui::DragFloat4("Bounding Box", &m_DebugOpt->boundArea.x))
    {
        m_DebugOpt->boundAreaShape->setSize({m_DebugOpt->boundArea.z, m_DebugOpt->boundArea.w});
    }

    if (ImGui::DragFloat("AgentSize", &m_DebugOpt->agentSize, 0.5f, 0.0f, 20.0f))
    {
        // m_DebugOpt->agentShape->setRadius(m_DebugOpt->agentSize);
        m_DebugOpt->agentShape->setSize({m_DebugOpt->agentSize,m_DebugOpt->agentSize});
        m_DebugOpt->agentShape->setOrigin(m_DebugOpt->agentSize, m_DebugOpt->agentSize);
    }
    
    glm::fvec2 mousePos{GetWorld().GetInputSettings().GetMouseEnginePos()};
    
    ImGui::DragFloat2("Mouse Engine Position", &mousePos.x);

    if(GetWorld().GetInputSettings().GetInputAction("DecreaseEntities",InputEvent::IE_Down))
    {
        
        m_ActiveAgents--;
        
        const int agentDiff{int(m_DebugOpt->activeAgents - m_ActiveAgents)};
        if (agentDiff < 0)
        {
            // remove
            for (int i = m_DebugOpt->activeAgents; i < m_ActiveAgents; ++i)
            {
                const auto& gameObject{m_SceneComponent->agentObjects[i]};
                VelocityComponent* vel = gameObject->AddComponent(VelocityComponent{});
                vel->maxVelocity = Random::RandFloat(25.0f, 100.0f);

                DrawAsset* draw{ gameObject->AddComponent(DrawAsset{})};
                draw->drawAsset = m_DebugOpt->agentShape.get();
            }
        }
        else if (agentDiff > 0)
        {
            for (int i = m_ActiveAgents; i < m_DebugOpt->activeAgents; ++i)
            {
                const auto& gameObject{m_SceneComponent->agentObjects[i]};
                gameObject->RemoveComponent<VelocityComponent>();
                gameObject->RemoveComponent<DrawAsset>();
            }
        }

        m_DebugOpt->activeAgents = m_ActiveAgents;
    }
    
    if (ImGui::DragInt("Agent Amount", &m_ActiveAgents, 1.0f, 0,
                       int(m_SceneComponent->agentObjects.size()), "%d", ImGuiSliderFlags_Logarithmic))
    {
        const int agentDiff{int(m_DebugOpt->activeAgents - m_ActiveAgents)};
        if (agentDiff < 0)
        {
            // remove
            for (int i = m_DebugOpt->activeAgents; i < m_ActiveAgents; ++i)
            {
                const auto& gameObject{m_SceneComponent->agentObjects[i]};
                VelocityComponent* vel = gameObject->AddComponent(VelocityComponent{});
                vel->maxVelocity = Random::RandFloat(25.0f, 100.0f);

#ifdef EntityDraw
                DrawAsset* draw{ gameObject->AddComponent(DrawAsset{})};
                draw->drawAsset = m_DebugOpt->agentShape.get();
#endif
            }
        }
        else if (agentDiff > 0)
        {
            for (int i = m_ActiveAgents; i < m_DebugOpt->activeAgents; ++i)
            {
                const auto& gameObject{m_SceneComponent->agentObjects[i]};
                gameObject->RemoveComponent<VelocityComponent>();
#ifdef EntityDraw
                gameObject->RemoveComponent<DrawAsset>();
#endif
            }
        }
        
        m_DebugOpt->activeAgents = m_ActiveAgents;
    }

    ImGui::End();

#endif
}
