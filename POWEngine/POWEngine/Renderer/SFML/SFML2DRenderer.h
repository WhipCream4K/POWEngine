#pragma once

#include <SFML/Graphics/PrimitiveType.hpp>

#include "POWEngine/Renderer/RenderAPI.h"
#include "POWEngine/Core/CustomTypes.h"

namespace sf
{
    class Vertex;
    class VertexArray;
    class RenderStates;
    class Drawable;
}

namespace powe
{
    class SFML2DRenderer : public RenderAPI
    {
    public:
        
        void SubmitRenderEntity(const sf::Drawable& drawable,const sf::RenderStates& renderStates) const;
        void SubmitRenderEntity(const sf::Vertex* drawable,size_t vertexCount,sf::PrimitiveType primitive) const;
        void SubmitRenderEntity(const sf::VertexArray& drawable) const;
        
        void OnChangeWindow(const Window&) override;
        void OnWindowCreate(const Window& window) override;
        void DisplayBuffer(const Window&) override;
        void ClearBackBuffer() override;
        void SetClearColor(const glm::uvec4&) override;

        SFML2DRenderer();
        SFML2DRenderer(const SFML2DRenderer&) = delete;
        SFML2DRenderer& operator=(const SFML2DRenderer&) = delete;
        SFML2DRenderer(SFML2DRenderer&&) noexcept = delete;
        SFML2DRenderer& operator=(SFML2DRenderer&&) noexcept = delete;
        ~SFML2DRenderer() override;
        
        
    private:

        class SFML2DRendererImpl;
        OwnedPtr<SFML2DRendererImpl> m_RendererImpl;
    };
}

