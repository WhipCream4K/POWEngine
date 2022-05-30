#pragma once

#include "POWEngine/Rendering/SFML/SFML2DRenderer.h"
#include "POWEngine/Rendering/System/RenderSystemBase.h"

namespace powe
{
	class SFML2DBoundingBoxRender : public RenderSystem<SFML2DRenderer>
	{
	public:

		SFML2DBoundingBoxRender();
		void OnDraw(const SFML2DRenderer& renderer, GameObjectID id) override;
	};

}


