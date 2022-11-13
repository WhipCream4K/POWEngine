#pragma once

#include "POWEngine/Renderer/SFML/SFML2DRenderer.h"
#include "POWEngine/Renderer/System/RenderSystemBase.h"

namespace powe
{
    class SFMLDefaultRenderSystem : public RenderSystem<SFML2DRenderer>
    {
    public:
        SFMLDefaultRenderSystem();
        void OnDraw(const SFML2DRenderer& renderer, const Window& renderWindow, GameObjectID id) override;
    };
}

