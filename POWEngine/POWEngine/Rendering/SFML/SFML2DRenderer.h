#pragma once

#include "POWEngine/Rendering/RendererImpl.h"

#if USE_SFML_RENDERER

namespace sf
{
	class Drawable;
	class RenderStates;
}

namespace powe
{
	class SFML2DRenderer :
		public RendererImpl
	{
	public:

		SFML2DRenderer(uint32_t width, uint32_t height, const OtherWindowParams& others);
		SFML2DRenderer(const SFML2DRenderer&) = delete;
		SFML2DRenderer& operator=(const SFML2DRenderer&) = delete;
		SFML2DRenderer(SFML2DRenderer&&) = delete;
		SFML2DRenderer& operator=(SFML2DRenderer&&) = delete;

		void PushRenderBuffer(RawByte* buffer, uint32_t size) override;
		void DrawBufferOnWindow(const Window& window) override;

	private:

		struct DrawEntity
		{
			sf::Drawable* instance{};
			sf::RenderStates* renderStates{};
			int layer{};
		};

		std::vector<DrawEntity> m_DrawEntities;
	};
}

#endif


