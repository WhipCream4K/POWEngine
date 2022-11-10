#pragma once
#include "POWEngine/ECS/SystemBase.h"

class ResetRenderObjectSystem : public powe::SystemBase
{
public:
    ResetRenderObjectSystem();
protected:
    void OnUpdate(float, powe::GameObjectID) override;
};
