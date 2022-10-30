#include "BoundAreaSystem.h"

#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/ECS/ECSUtils.h"

BoundAreaSystem::BoundAreaSystem()
{
    // DEFINE_SYSTEM_KEY(powe::Transform2D);
    DefineSystemKeys<powe::Transform2D>();
}

void BoundAreaSystem::OnUpdate(float, powe::GameObjectID)
{
    
}
