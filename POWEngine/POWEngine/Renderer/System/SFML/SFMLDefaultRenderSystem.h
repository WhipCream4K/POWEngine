#pragma once

#include "POWEngine/Renderer/SFML/SFML2DRendererThreaded.h"
#include "POWEngine/Renderer/System/RenderSystemBase.h"

namespace powe
{
    class SFMLDefaultRenderSystem : public RenderSystemBase<SFML2DRendererThreaded>
    {
    public:
        SFMLDefaultRenderSystem();
        void OnDraw(const SFML2DRendererThreaded& renderer, const Window& renderWindow, GameObjectID id) override;
    };
}

