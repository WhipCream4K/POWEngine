#pragma once

#include "POWEngine/Renderer/RenderAPI.h"
#include "POWEngine/Core/CustomTypes.h"

//#if USE_SFML_RENDERER

namespace sf
{
	class Drawable;
	class RenderStates;
}

namespace powe
{
	class SFML2DRenderer : public RenderAPI
		
	{
	public:

		SFML2DRenderer();
		SFML2DRenderer(const SFML2DRenderer&) = delete;
		SFML2DRenderer& operator=(const SFML2DRenderer&) = delete;
		SFML2DRenderer(SFML2DRenderer&&) = delete;
		SFML2DRenderer& operator=(SFML2DRenderer&&) = delete;
		~SFML2DRenderer() override;

		void SubmitDrawSprite(sf::Drawable* drawable,const sf::RenderStates& renderStates,int drawOrder) const; 
		void DrawBufferOnWindow(const Window& window) const override;

	private:

		class SFML2DRendererImpl;
		OwnedPtr<SFML2DRendererImpl> m_RenderImpl;
	};
}




