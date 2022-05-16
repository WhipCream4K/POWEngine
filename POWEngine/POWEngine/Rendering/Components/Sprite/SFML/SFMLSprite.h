#pragma once


#include "POWEngine/Rendering/Components/Sprite/SpriteImpl.h"
#include "POWEngine/Core/Components/BaseComponent.h"

#if USE_SFML_RENDERER
#include <SFML/Graphics.hpp>

namespace powe
{
	class SFMLSprite : public SpriteImpl
	{
	public:

		SFMLSprite() = default;
		SFMLSprite(WorldEntity* worldEntity, GameObjectID id);
		void SetOrigin(float x, float y) override;
		void SetTexture(const Texture& texture) override;
		void SetOrigin(const glm::fvec2& pos) override;
		void SetRect(const glm::fvec4& rect) override;

	private:

		WorldEntity* m_pWorld{};
		GameObjectID m_OwnerID{};
	};
}

#endif

