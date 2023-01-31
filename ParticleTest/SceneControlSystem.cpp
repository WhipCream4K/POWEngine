#include "SceneControlSystem.h"

#include "UserComponents.h"
#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Core/GameObject/GameObject.h"
#include "POWEngine/Logger/Logger.h"
#include "POWEngine/Logger/LoggerUtils.h"
#include "POWEngine/Random/Random.h"
#include "POWEngine/Renderer/Components/Debug2D/SFML/SFML2DShapeComponent.h"

#include "POWEngine/Debug/imgui/ImGUI.h"


using namespace powe;

SceneControlSystem::SceneControlSystem()
{
    DefineSystemKeys<SceneComponent, DebugSteeringComponent>();
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

    if(!m_AgentVerticesBatch)
    {
        m_AgentVerticesBatch = world.FindUniqueComponent<AgentVerticesBatch>();
    }
    
    if (!m_SceneComponent || !m_DebugOpt || !m_EngineComponent || !m_AgentVerticesBatch)
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
    m_AgentVerticesBatch->vertexID = 0;
    m_AgentVerticesBatch->vertexBuffer.clear();

#if USE_IMGUI
    
    ImGui::Begin("Debug Window");

    ImGui::DragFloat("FrameTime",&deltaTime);

    ImGui::DragInt("FPS",&m_EngineComponent->fps);
    
    if (ImGui::DragFloat4("Bounding Box", &m_DebugOpt->boundArea.x))
    {
        m_DebugOpt->boundAreaShape->setSize({m_DebugOpt->boundArea.z, m_DebugOpt->boundArea.w});
    }

    if (ImGui::DragFloat("AgentSize", &m_AgentVerticesBatch->rectSize, 0.5f, 0.0f, 20.0f))
    {
        // m_DebugOpt->agentShape->setRadius(m_DebugOpt->agentSize);
        // m_DebugOpt->agentShape->setSize({m_DebugOpt->agentSize,m_DebugOpt->agentSize});
        // m_DebugOpt->agentShape->setOrigin(m_DebugOpt->agentSize, m_DebugOpt->agentSize);
    }
    
    glm::fvec2 mousePos{GetWorld().GetInputSettings().GetMouseEnginePos()};
    
    ImGui::DragFloat2("MouseEnginePos", &mousePos.x);

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

    ImGui::End();

#endif
}
