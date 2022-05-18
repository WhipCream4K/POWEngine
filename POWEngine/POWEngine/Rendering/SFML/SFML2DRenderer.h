#pragma once

#include "POWEngine/Rendering/Renderer.h"

#if USE_SFML_RENDERER

namespace sf
{
	class Drawable;
	class RenderStates;
}

namespace powe
{
	class SFML2DRenderer : public Renderer
		
	{
	public:

		SFML2DRenderer(uint32_t width, uint32_t height, const OtherWindowParams& others);
		SFML2DRenderer(const SFML2DRenderer&) = delete;
		SFML2DRenderer& operator=(const SFML2DRenderer&) = delete;
		SFML2DRenderer(SFML2DRenderer&&) = delete;
		SFML2DRenderer& operator=(SFML2DRenderer&&) = delete;

		void SubmitDrawSprite(sf::Drawable* drawable,sf::RenderStates* renderStates,int drawOrder) const; 
		void DrawBufferOnWindow(const Window& window) const override;

	private:

		class SFML2DRendererImpl;
		OwnedPtr<SFML2DRendererImpl> m_RenderImpl;
	};
}

#endif


