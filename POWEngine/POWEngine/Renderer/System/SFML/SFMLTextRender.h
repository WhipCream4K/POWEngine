#pragma once

#include "POWEngine/Renderer/SFML/SFML2DRenderer.h"
#include "POWEngine/Renderer/System/RenderSystemBase.h"

namespace powe
{
	class SFMLTextRender : public RenderSystem<SFML2DRenderer>
	{
	public:

		SFMLTextRender();
		// void OnDraw(const SFML2DRenderer& renderer, GameObjectID id) override;
	};
}


