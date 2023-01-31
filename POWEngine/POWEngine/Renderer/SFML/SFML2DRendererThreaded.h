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
	class SFML2DRendererThreaded : public RenderAPI
	{
	public:
		class SFML2DRendererImpl;

		SFML2DRendererThreaded();
		// SFML2DRenderer(Window& window);
		SFML2DRendererThreaded(const SFML2DRendererThreaded&) = delete;
		SFML2DRendererThreaded& operator=(const SFML2DRendererThreaded&) = delete;
		SFML2DRendererThreaded(SFML2DRendererThreaded&&) = delete;
		SFML2DRendererThreaded& operator=(SFML2DRendererThreaded&&) = delete;
		~SFML2DRendererThreaded() override;

		void SubmitDrawSprite(sf::Drawable* drawable,const sf::RenderStates& renderStates,int drawOrder) const;
		void SubmitRenderEntity(const sf::Drawable& drawable,const sf::RenderStates& renderStates) const;

		void OnChangeWindow(const Window&) override;
		void OnWindowResize(const Window& window) override;
		void OnWindowCreate(const Window& window) override;
		void DisplayBuffer(const Window&) override;
		void DeferredDrawOnWindow(const Window&) override;
		void ClearBackBuffer() override;

	private:

		OwnedPtr<SFML2DRendererImpl> m_RenderImpl;

	};
}




