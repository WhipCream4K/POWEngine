#include "SceneControlSystem.h"

#include <imgui-SFML.h>
#include <imgui.h>

#include "UserComponents.h"
#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Core/GameObject/GameObject.h"
#include "POWEngine/Logger/Logger.h"
#include "POWEngine/Logger/LoggerUtils.h"
#include "POWEngine/Random/Random.h"
#include "POWEngine/Renderer/Components/Debug2D/SFML/SFML2DShapeComponent.h"


using namespace powe;

SceneControlSystem::SceneControlSystem()
{
    DefineSystemKeys<SceneComponent,DebugSteeringComponent>();
}

void SceneControlSystem::OnCreate(powe::GameObjectID id [[maybe_unused]])
{
    const auto& world{GetWorld()};
    
    if(!m_DebugOpt)
    {
        m_DebugOpt = world.GetComponent<DebugSteeringComponent>(id);
        m_ActiveAgents = m_DebugOpt->activeAgents;
    }

    if(!m_DrawBoundingBox)
        m_DrawBoundingBox = world.GetComponent<SFML2DRectangle>(id);

    if(!m_SceneComponent)
        m_SceneComponent = world.GetComponent<SceneComponent>(id);
    
    if(!m_SceneComponent || !m_DebugOpt || !m_DrawBoundingBox)
        POWLOGERROR("No Scene Control Data");
}

void SceneControlSystem::OnUpdate(float, powe::GameObjectID)
{
    ImGui::Begin("Debug Window");

    if(ImGui::DragFloat4("Bounding Box",&m_DebugOpt->boundArea.x))
    {
        m_DrawBoundingBox->SetSize({m_DebugOpt->boundArea.z,m_DebugOpt->boundArea.w});
    }

    if(ImGui::DragFloat("AgentSize",&m_DebugOpt->agentSize,0.5f,0.0f,20.0f))
    {
        for (int i = 0; i < m_DebugOpt->activeAgents; ++i)
        {
            const auto& gameObject{m_SceneComponent->drawObjects[i]};
            SFML2DCircle* draw{gameObject->GetComponent<SFML2DCircle>()};
            draw->SetSize({m_DebugOpt->agentSize,m_DebugOpt->agentSize});
        }
    }
    
    glm::fvec2 mousePos{ GetWorld().GetInputSettings().GetMouseEnginePos()};
    ImGui::DragFloat2("MouseEnginePos",&mousePos.x);

    if(ImGui::DragInt("Agent Amount",&m_ActiveAgents,1.0f,0,
        int(m_SceneComponent->agentObjects.size()),"%d",ImGuiSliderFlags_Logarithmic))
    {
        const int agentDiff{int(m_DebugOpt->activeAgents - m_ActiveAgents)};
        if(agentDiff < 0)
        {
            // remove
            for (int i = m_DebugOpt->activeAgents; i < m_ActiveAgents; ++i)
            {
                const auto& gameObject{m_SceneComponent->agentObjects[i]};
                VelocityComponent* vel = gameObject->AddComponent(VelocityComponent{});
                vel->maxVelocity = Random::RandFloat(25.0f, 100.0f);
                SFML2DCircle* draw{gameObject->AddComponent(SFML2DCircle{GetWorld(),gameObject->GetID()})};

                draw->SetSize({m_DebugOpt->agentSize,m_DebugOpt->agentSize});
            }
        }
        else if(agentDiff > 0)
        {
            for (int i = m_ActiveAgents; i < m_DebugOpt->activeAgents; ++i)
            {
                const auto& gameObject{m_SceneComponent->agentObjects[i]};
                gameObject->RemoveComponent<VelocityComponent>();
                gameObject->RemoveComponent<SFML2DCircle>();
            }
        }

        m_DebugOpt->activeAgents = m_ActiveAgents;
    }

    ImGui::End();
}
