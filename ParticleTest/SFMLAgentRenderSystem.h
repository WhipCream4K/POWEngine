#pragma once

#include "UserComponents.h"
#include "POWEngine/Renderer/SFML/SFML2DRenderer.h"
#include "POWEngine/Renderer/System/RenderSystemBase.h"

namespace powe
{
    class GameObject;
}

class SFMLAgentRenderSystem : public powe::RenderSystem<powe::SFML2DRenderer>
{
public:
    SFMLAgentRenderSystem();
protected:
    void OnDraw(const powe::SFML2DRenderer& renderer, const powe::Window& renderWindow, powe::GameObjectID id) override;
};
