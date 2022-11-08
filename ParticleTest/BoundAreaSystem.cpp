#include "BoundAreaSystem.h"

#include "POWEngine/Core/Components/Transform2D.h"

using namespace powe;

BoundAreaSystem::BoundAreaSystem(const glm::fvec4& box)
    : m_BoundingBox(box * 0.5f)
{
    DefineSystemKeys<Transform2D>();
}

void BoundAreaSystem::OnUpdate(float, powe::GameObjectID)
{
    Transform2D* transform{GetComponent<Transform2D>()};

    auto position{transform->GetPosition()};


    // bound horizontal
    if (position.x < m_BoundingBox.x - m_BoundingBox.z - 2.0f)
    {
        position.x = m_BoundingBox.x + m_BoundingBox.z;
    }
    else if (position.x > m_BoundingBox.x + m_BoundingBox.z + 2.0f)
    {
        position.x = m_BoundingBox.x - m_BoundingBox.z;
    }

    if(position.y < m_BoundingBox.y - m_BoundingBox.w  - 2.0f)
    {
        position.y = m_BoundingBox.y + m_BoundingBox.w;
    }
    else if (position.y > m_BoundingBox.y + m_BoundingBox.w + 2.0f)
    {
        position.y = m_BoundingBox.y - m_BoundingBox.w;
    }

    transform->SetWorldPosition(position);
}
