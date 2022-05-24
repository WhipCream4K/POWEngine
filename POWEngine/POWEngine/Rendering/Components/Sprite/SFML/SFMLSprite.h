#pragma once


#include "POWEngine/Rendering/Components/Sprite/SpriteImpl.h"


#include <SFML/Graphics.hpp>

namespace powe
{
	struct SFMLSpriteComponent;
	class SFMLSprite : public SpriteImpl
	{
	public:

		SFMLSprite();
		SFMLSprite(WorldEntity* worldEntity, GameObjectID id);
		~SFMLSprite() override;
		void SetOrigin(float x, float y) override;
		void SetTexture(const Texture& texture) override;
		void SetOrigin(const glm::fvec2& pos) override;
		void SetRect(const glm::fvec4& rect) override;
		SFMLSpriteComponent* GetSfSprite() const;

	private:

		WorldEntity* m_pWorld{};
		GameObjectID m_OwnerID{};
	};
}


