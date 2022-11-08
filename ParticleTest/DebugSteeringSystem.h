#pragma once

#include "POWEngine/Renderer/SFML/SFML2DRenderer.h"
#include "POWEngine/Renderer/System/RenderSystemBase.h"

class DebugSteeringSystem : public powe::RenderSystem<powe::SFML2DRenderer>
{
public:
    
    DebugSteeringSystem();
    
protected:
    
    void OnDraw(const powe::SFML2DRenderer& renderer,
        const powe::Window& renderWindow, powe::GameObjectID id) override;
};
