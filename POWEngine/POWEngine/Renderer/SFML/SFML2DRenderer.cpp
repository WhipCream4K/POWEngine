#include "pch.h"
#include "SFML2DRenderer.h"

#include <SFML/Graphics.hpp>

#include "POWEngine/Logger/LoggerUtils.h"
#include "POWEngine/Utils/Utils.h"
#include "POWEngine/Window/SFML/SFMLWindow.h"

#include "POWEngine/Debug/imgui/ImGUI.h"

class powe::SFML2DRenderer::SFML2DRendererImpl
{
public:
    
    void SetTargetWindow(const Window& window);
    void ClearBackBuffer() const;
    void SubmitRenderEntity(const sf::Drawable& drawable,const sf::RenderStates& renderStates) const;
    void SubmitRenderEntity(const sf::Vertex* drawable,size_t vertexCount,sf::PrimitiveType primitive) const;
    void SubmitRenderEntity(const sf::VertexArray& vertexArray) const;
    void DisplayBuffer(const Window& window) const;
    void SetClearColor(const sf::Color& color);
    sf::RenderWindow* GetFocusWindow() const {return m_TargetRenderWindow;}
    
private:
    
    sf::RenderWindow* m_TargetRenderWindow{};
    sf::Color m_ClearColor{};
};

void powe::SFML2DRenderer::SFML2DRendererImpl::SetTargetWindow(const Window& window)
{
    if(const auto sfmlWindow = window.GetWindowInstance<SFMLWindow>(); sfmlWindow)
    {
        m_TargetRenderWindow = &sfmlWindow->GetRenderWindow();
    }
    else
    {
        POWLOGERROR("No sfml render window instance");
    }
}

void powe::SFML2DRenderer::SFML2DRendererImpl::ClearBackBuffer() const
{
    m_TargetRenderWindow->clear(m_ClearColor);
}

void powe::SFML2DRenderer::SFML2DRendererImpl::SubmitRenderEntity(const sf::Drawable& drawable, 
    const sf::RenderStates& renderStates) const
{
    m_TargetRenderWindow->draw(drawable,renderStates);
}

void powe::SFML2DRenderer::SFML2DRendererImpl::SubmitRenderEntity(
    const sf::Vertex* drawable,size_t vertexCount,sf::PrimitiveType primitive) const
{
    m_TargetRenderWindow->draw(drawable,vertexCount,primitive);
}

void powe::SFML2DRenderer::SFML2DRendererImpl::SubmitRenderEntity(const sf::VertexArray& vertexArray) const
{
    m_TargetRenderWindow->draw(vertexArray);
}

void powe::SFML2DRenderer::SFML2DRendererImpl::DisplayBuffer(const Window&) const
{
    m_TargetRenderWindow->display();
}

void powe::SFML2DRenderer::SFML2DRendererImpl::SetClearColor(const sf::Color& color)
{
    m_ClearColor = color;
}

void powe::SFML2DRenderer::SubmitRenderEntity(const sf::Drawable& drawable, const sf::RenderStates& renderStates) const
{
    m_RendererImpl->SubmitRenderEntity(drawable,renderStates);
}

void powe::SFML2DRenderer::SubmitRenderEntity(const sf::Vertex* drawable, size_t vertexCount,
    sf::PrimitiveType primitive) const
{
    m_RendererImpl->SubmitRenderEntity(drawable,vertexCount,primitive);
}

void powe::SFML2DRenderer::SubmitRenderEntity(const sf::VertexArray& drawable) const
{
    m_RendererImpl->SubmitRenderEntity(drawable);
}

void powe::SFML2DRenderer::OnChangeWindow(const Window& window)
{
    m_RendererImpl->SetTargetWindow(window);
}

void powe::SFML2DRenderer::OnWindowCreate(const Window& window)
{
    m_RendererImpl->SetTargetWindow(window);
}

void powe::SFML2DRenderer::DisplayBuffer(const Window& window)
{
    m_RendererImpl->DisplayBuffer(window);
}

void powe::SFML2DRenderer::ClearBackBuffer()
{
    m_RendererImpl->ClearBackBuffer();
}

void powe::SFML2DRenderer::SetClearColor(const glm::uvec4& color)
{
    m_RendererImpl->SetClearColor(sf::ConvertToSFColor(color));
}

sf::RenderWindow* powe::SFML2DRenderer::GetRenderWindow() const
{
    return m_RendererImpl->GetFocusWindow();
}

powe::SFML2DRenderer::SFML2DRenderer()
    : m_RendererImpl(std::make_unique<SFML2DRendererImpl>())
{
}

powe::SFML2DRenderer::~SFML2DRenderer() = default;
