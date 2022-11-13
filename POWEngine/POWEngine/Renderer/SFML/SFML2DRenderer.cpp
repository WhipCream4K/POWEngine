#include "pch.h"
#include "SFML2DRenderer.h"

//#if USE_SFML_RENDERER

#include "POWEngine/Window/Window.h"
#include "POWEngine/Window/SFML/SFMLWindow.h"
#include "POWEngine/Logger/LoggerUtils.h"
#include <SFML/Graphics.hpp>

class powe::SFML2DRenderer::SFML2DRendererImpl
{
public:

	void DrawBufferOnWindow(const Window& window)
	{
		SFMLWindow* sfmlWindow{ window.GetWindowInstance<SFMLWindow>() };
		sf::RenderWindow& renderWindow{ sfmlWindow->GetRenderWindow() };

		// sort the render order
		// std::ranges::sort(m_SF2DDrawEntities, [](const DrawEntity& left, const DrawEntity& right)
		// 	{
		// 		return left.drawOrder < right.drawOrder;
		// 	});
		
		for (const auto& drawEntt : m_SF2DDrawEntities)
		{
			//POWLOGERROR(std::to_string(m_SF2DDrawEntities.size()));
			renderWindow.draw(*drawEntt.sfDraw, drawEntt.sfRenderStates);
		}
		
		m_SF2DDrawEntities.clear();
	}

	void SubmitDrawSprite(sf::Drawable* drawObject, const sf::RenderStates& renderStates, int drawOrder)
	{
		if (drawObject)
			m_SF2DDrawEntities.emplace_back(drawObject, renderStates, drawOrder);
	}

private:

	struct DrawEntity
	{
		sf::Drawable* sfDraw{};
		sf::RenderStates sfRenderStates{};
		int drawOrder{};
	};

	std::vector<DrawEntity> m_SF2DDrawEntities;
};

powe::SFML2DRenderer::SFML2DRenderer()
	: RenderAPI()
	, m_RenderImpl(std::make_unique<SFML2DRendererImpl>())
{
}

powe::SFML2DRenderer::~SFML2DRenderer() = default;

void powe::SFML2DRenderer::SubmitDrawSprite(sf::Drawable* drawable, const sf::RenderStates& renderStates, int drawOrder) const
{
	m_RenderImpl->SubmitDrawSprite(drawable, renderStates, drawOrder);
}

void powe::SFML2DRenderer::DrawBufferOnWindow(const Window& window) const
{
	m_RenderImpl->DrawBufferOnWindow(window);
}


//#endif