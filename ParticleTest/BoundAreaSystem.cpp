#include "BoundAreaSystem.h"

#include "POWEngine/Core/Components/Transform2D.h"

BoundAreaSystem::BoundAreaSystem()
{
    DEFINE_SYSTEM_KEY(powe::Transform2D);
}

void BoundAreaSystem::OnUpdate(float, powe::GameObjectID)
{
    
}
