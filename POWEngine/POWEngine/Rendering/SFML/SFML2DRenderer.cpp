#include "pch.h"
#include "SFML2DRenderer.h"

#include "POWEngine/Rendering/RenderBridge.h"

#if USE_SFML_RENDERER

#include "POWEngine/Window/Window.h"
#include "POWEngine/Window/SFML/SFMLWindow.h"
#include <SFML/Graphics.hpp>

powe::SFML2DRenderer::SFML2DRenderer(uint32_t width, uint32_t height, const OtherWindowParams& others)
	: RendererImpl(width,height,others)
{
}


void powe::SFML2DRenderer::PushRenderBuffer(RawByte* buffer, uint32_t)
{
	//SFMLSpriteComponent* sfDrawInst{ reinterpret_cast<SFMLSpriteComponent*>(buffer) };
	//m_DrawEntities.emplace_back(&sfDrawInst->sprite, 0);
}

void powe::SFML2DRenderer::DrawBufferOnWindow(const Window& window)
{
	// ordering the z depths of the entities and render them back to forth
	if (SFMLWindow * windowSFML{ window.GetWindowInstance<SFMLWindow>() })
	{
		// sort
		std::ranges::sort(m_DrawEntities, [](const DrawEntity& left, const DrawEntity& right)
			{
				return left.layer < right.layer;
			});

		for (const auto& drawEntt : m_DrawEntities)
		{
			windowSFML->GetRenderWindow().draw(*drawEntt.instance);
		}
	}
}

#endif