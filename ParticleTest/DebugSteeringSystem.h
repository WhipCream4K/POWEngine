#pragma once

#include "POWEngine/Renderer/SFML/SFML2DRenderer.h"
#include "POWEngine/Renderer/System/RenderSystemBase.h"

class DebugSteeringSystem : public powe::SystemBase
{
public:
    
    DebugSteeringSystem();
    
protected:
    
    void OnUpdate(float, powe::GameObjectID) override;
};
