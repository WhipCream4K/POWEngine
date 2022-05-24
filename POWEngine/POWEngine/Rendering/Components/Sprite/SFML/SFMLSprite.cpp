#include "pch.h"
#include "SFMLSprite.h"

#include "POWEngine/Core/WorldEntity/WorldEntity.h"
#include "SFMLSpriteComponent.h"
#include "POWEngine/Rendering/Resources/Texture/Texture.h"
#include "POWEngine/Rendering/Resources/Texture/SFML/SFMLTexture.h"
#include "POWEngine/Core/GameObject/GameObject.h"


powe::SFMLSprite::SFMLSprite()
	: m_Owner()
{
}

powe::SFMLSprite::SFMLSprite(const SharedPtr<GameObject>& owner)
	: m_Owner(owner)
{
	if(owner)
	{
		owner->AddComponent(SFMLSpriteComponent{}, ComponentFlag::Sparse);
	}
}

powe::SFMLSprite::~SFMLSprite() = default;

void powe::SFMLSprite::SetOrigin(float x, float y)
{
	if (const auto gameObject{ m_Owner.lock() })
	{
		if (SFMLSpriteComponent * sfmlSprite{ gameObject->GetComponent<SFMLSpriteComponent>() })
		{
			sfmlSprite->sprite.setOrigin({ x,y });
		}
	}
}

void powe::SFMLSprite::SetTexture(const Texture& texture)
{
	if(const auto gameObject{m_Owner.lock()})
	{
		if (SFMLSpriteComponent * sfmlSprite{ gameObject->GetComponent<SFMLSpriteComponent>() })
		{
			if (SFMLTexture * sfmlTexture{ texture.GetTypeInstance<SFMLTexture>() })
			{
				sfmlSprite->sprite.setTexture(sfmlTexture->GetSFTexture(), false);
			}
		}
	}
}

void powe::SFMLSprite::SetOrigin(const glm::fvec2& pos)
{
	SetOrigin(pos.x, pos.y);
}

void powe::SFMLSprite::SetRect(const glm::fvec4& rect)
{
	if(const auto gameObject{m_Owner.lock()})
	{
		if (SFMLSpriteComponent * sfmlSprite{ gameObject->GetComponent<SFMLSpriteComponent>() })
		{
			sfmlSprite->sprite.setTextureRect(sf::IntRect{ {int(rect.x),int(rect.y)},{int(rect.z),int(rect.w)} });
		}
	}

}

powe::SFMLSpriteComponent* powe::SFMLSprite::GetSfSprite() const
{
	const auto gameObject{ m_Owner.lock() };
	return gameObject ? nullptr : gameObject->GetComponent<SFMLSpriteComponent>();
}
