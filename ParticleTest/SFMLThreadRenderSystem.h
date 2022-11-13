#pragma once
#include <SFML/Graphics.hpp>

#include "POWEngine/Renderer/SFML/SFML2DRenderer.h"
#include "POWEngine/Renderer/System/RenderSystemBase.h"

class SFMLThreadRenderSystem : public powe::RenderSystem<powe::SFML2DRenderer>
{
public:
    
    SFMLThreadRenderSystem();
    
protected:
    
    void OnDraw(const powe::SFML2DRenderer& renderer, const powe::Window& renderWindow, powe::GameObjectID id) override;
    
    sf::RenderTexture m_RenderTexture;
    int m_EntityCount{};
};
