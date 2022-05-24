#pragma once

#include "POWEngine/Rendering/System/RenderSystemBase.h"
#include "POWEngine/Rendering/SFML/SFML2DRenderer.h"

class SFMLSpriteRender : public powe::RenderSystem<powe::SFML2DRenderer>
{
public:

	SFMLSpriteRender();

protected:

	void OnDraw(const powe::WorldEntity& worldEntity, const powe::SFML2DRenderer& renderer, powe::GameObjectID id) override;


};

