#include "DebugSteeringSystem.h"

#include <imgui-SFML.h>
#include <imgui.h>

#include "UserComponents.h"
#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Renderer/Components/Debug2D/SFML/SFML2DShapeComponent.h"


using namespace powe;

DebugSteeringSystem::DebugSteeringSystem()
{
    DefineSystemKeys<DebugSteeringComponent>();
}

void DebugSteeringSystem::OnCreate(powe::GameObjectID)
{
    if(!m_DebugOpt)
        m_DebugOpt = &GetComponent<DebugSteeringComponent>();

    if(!m_DrawBoundingBox)
        m_DrawBoundingBox = &GetComponent<SFML2DRectangle>();
}

void DebugSteeringSystem::OnUpdate(float, powe::GameObjectID)
{
    
    ImGui::Begin("Debug Window");

    if(ImGui::DragFloat4("Bounding Box",&m_DebugOpt->boundArea.x))
    {
        m_DrawBoundingBox->SetSize({m_DebugOpt->boundArea.z,m_DebugOpt->boundArea.w});
        m_DebugOpt->isOptionsChanged = true;
    }

    if(ImGui::DragFloat("AgentSize",&m_DebugOpt->agentSize,0.5f,0.0f,20.0f))
    {
        m_DebugOpt->isOptionsChanged = true;
    }
    
    glm::fvec2 mousePos{ GetWorld()->GetInputSettings().GetMouseEngineAxisData()};
    ImGui::DragFloat2("MouseEnginePos",&mousePos.x);

    ImGui::DragInt("AgentAmount",&m_DebugOpt->agentAmount);

    ImGui::End();
}
