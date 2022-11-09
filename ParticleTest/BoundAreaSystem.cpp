#include "BoundAreaSystem.h"

#include <imgui-SFML.h>
#include <imgui.h>

#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Core/GameObject/GameObject.h"
#include "UserComponents.h"

using namespace powe;

BoundAreaSystem::BoundAreaSystem(const glm::fvec4& box)
    : m_BoundingBox(box * 0.5f)
{
    DefineSystemKeys<Transform2D>();
}

BoundAreaSystem::BoundAreaSystem(const SharedPtr<powe::GameObject>& gb)
    : m_BoundArea(gb)
{
    DefineSystemKeys<Transform2D>();
}

void BoundAreaSystem::OnCreate(powe::GameObjectID)
{
    if(!m_DebugSteeringOpt)
        m_DebugSteeringOpt = m_BoundArea.lock()->GetComponent<DebugSteeringComponent>();
}

void BoundAreaSystem::OnUpdate(float, powe::GameObjectID)
{
    Transform2D* transform{GetComponent<Transform2D>()};
    
    auto position{transform->GetPosition()};
    
    // bound horizontal
    const float left{m_DebugSteeringOpt->boundArea.x - m_DebugSteeringOpt->boundArea.z * 0.5f};
    const float right{m_DebugSteeringOpt->boundArea.x + m_DebugSteeringOpt->boundArea.z * 0.5f};
    const float up{m_DebugSteeringOpt->boundArea.y + m_DebugSteeringOpt->boundArea.w * 0.5f};
    const float down{m_DebugSteeringOpt->boundArea.y - m_DebugSteeringOpt->boundArea.w * 0.5f};
    
    if (position.x < left - 2.0f)
    {
        position.x = right;
    }
    else if (position.x > right + 2.0f)
    {
        position.x = left;
    }
    
    if(position.y < down  - 2.0f)
    {
        position.y = up;
    }
    else if (position.y > up + 2.0f)
    {
        position.y = down;
    }
    
    transform->SetWorldPosition(position);
}
