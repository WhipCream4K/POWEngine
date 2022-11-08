#pragma once

#include "POWEngine/Renderer/System/RenderSystemBase.h"
#include "POWEngine/Renderer/SFML/SFML2DRenderer.h"

namespace powe
{
	class SFML2DRenderSystem : public RenderSystem<SFML2DRenderer>
	{
	public:

		SFML2DRenderSystem();

	protected:

		// void OnDraw(const SFML2DRenderer& renderer, GameObjectID id) override;
	};
}


