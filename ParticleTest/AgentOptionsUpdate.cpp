﻿#include "AgentOptionsUpdate.h"

#include "UserComponents.h"
#include "POWEngine/Core/GameObject/GameObject.h"
#include "POWEngine/Renderer/Components/Debug2D/SFML/SFML2DShapeComponent.h"

using namespace powe;

AgentOptionsUpdate::AgentOptionsUpdate(const SharedPtr<powe::GameObject>& debugObject)
    : m_DebugObject(debugObject)
{
    DefineSystemKeys<VelocityComponent,powe::SFML2DCircle>();
}

void AgentOptionsUpdate::OnCreate(powe::GameObjectID)
{
    if(!m_DebugSteering)
    {
        m_DebugSteering = m_DebugObject.lock()->GetComponent<DebugSteeringComponent>();
    }
}

void AgentOptionsUpdate::OnUpdate(float, powe::GameObjectID)
{
    if(m_DebugSteering->isOptionsChanged)
    {
        SFML2DCircle* agentDraw{GetComponent<SFML2DCircle>()};
        agentDraw->SetSize({m_DebugSteering->agentSize,m_DebugSteering->agentSize});
    }
}