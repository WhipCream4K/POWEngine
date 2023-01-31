#pragma once
#include "POWEngine/Renderer/SFML/SFML2DRenderer.h"
#include "POWEngine/Renderer/System/RenderSystemBase.h"

namespace powe
{
    class SceneDraw : public powe::RenderSystemBase<SFML2DRenderer>
    {
    public:

        SceneDraw();

    protected:

        void OnDraw(const SFML2DRenderer& renderer, const Window& renderWindow, GameObjectID id) override;
        
    private:
    };
}


