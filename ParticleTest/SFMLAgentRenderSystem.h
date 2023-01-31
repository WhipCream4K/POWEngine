#pragma once

#include "UserComponents.h"
#include "POWEngine/Renderer/SFML/SFML2DRendererThreaded.h"
#include "POWEngine/Renderer/System/RenderSystemBase.h"

namespace powe
{
    class GameObject;
}

class SFMLAgentRenderSystem : public powe::RenderSystemBase<powe::SFML2DRendererThreaded>
{
public:
    SFMLAgentRenderSystem();
protected:
    void OnDraw(const powe::SFML2DRendererThreaded& renderer, const powe::Window& renderWindow, powe::GameObjectID id) override;
};
