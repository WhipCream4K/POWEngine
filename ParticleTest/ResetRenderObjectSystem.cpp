#include "ResetRenderObjectSystem.h"

#include "POWEngine/Core/Components/Transform2D.h"
#include "POWEngine/Core/Prefab/RenderTag.h"

ResetRenderObjectSystem::ResetRenderObjectSystem()
{
    DefineSystemKeys<powe::Transform2D,RenderTag>();
}

void ResetRenderObjectSystem::OnUpdate(float, powe::GameObjectID)
{
    const auto& transform{GetComponent<powe::Transform2D>()};
    transform->GetPosition();
}
