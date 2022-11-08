#pragma once

#include "POWEngine/Renderer/SFML/SFML2DRenderer.h"
#include "POWEngine/Renderer/System/RenderSystemBase.h"

namespace powe
{
    class SFML2DShapeRenderSystem : public powe::RenderSystem<SFML2DRenderer>
    {
    public:

        SFML2DShapeRenderSystem();
         // void OnDraw(const powe::SFML2DRenderer& renderer, powe::GameObjectID id) override;
    private:
        
    };
}

