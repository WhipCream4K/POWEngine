#include "pch.h"
#include "SFMLSprite.h"

#include "POWEngine/Core/WorldEntity/WorldEntity.h"
#include "POWEngine/Rendering/RenderBridge.h"
#include "POWEngine/Rendering/Resources/Texture/Texture.h"
#include "POWEngine/Rendering/Resources/Texture/SFML/SFMLTexture.h"

#if USE_SFML_RENDERER

powe::SFMLSprite::SFMLSprite(WorldEntity* worldEntity, GameObjectID id)
	: SpriteImpl(worldEntity, id)
	, m_pWorld(worldEntity)
	, m_OwnerID(id)
{
	if (worldEntity)
		worldEntity->AddComponentToGameObject(id, SFMLSpriteComponent{}, ComponentFlag::Sparse);
}

void powe::SFMLSprite::SetOrigin(float x, float y)
{
	if(SFMLSpriteComponent* sfmlSprite{ m_pWorld->GetComponent<SFMLSpriteComponent>(m_OwnerID) })
	{
		sfmlSprite->sprite.setOrigin({ x,y });
	}
}

void powe::SFMLSprite::SetTexture(const Texture& texture)
{
	if (SFMLSpriteComponent* sfmlSprite{ m_pWorld->GetComponent<SFMLSpriteComponent>(m_OwnerID) })
	{
		if(SFMLTexture* sfmlTexture{ texture.GetTypeInstance<SFMLTexture>() })
		{
			sfmlSprite->sprite.setTexture(sfmlTexture->GetSFTexture(), false);
		}
	}
}

void powe::SFMLSprite::SetOrigin(const glm::fvec2& pos)
{
	SetOrigin(pos.x, pos.y);
}

void powe::SFMLSprite::SetRect(const glm::fvec4& rect)
{
	if (SFMLSpriteComponent * sfmlSprite{ m_pWorld->GetComponent<SFMLSpriteComponent>(m_OwnerID) })
	{
		sfmlSprite->sprite.setTextureRect(sf::IntRect{{int(rect.x),int(rect.y)},{int(rect.z),int(rect.w)}});
	}
}

#endif