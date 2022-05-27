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
				const sf::Texture& sfTexture{ sfmlTexture->GetSFTexture() };
				sfmlSprite->sprite.setTexture(sfTexture, false);

				const sf::Vector2u textureSize{ sfTexture.getSize() };
				sfmlSprite->sprite.setOrigin({float(textureSize.x) / 2.0f,float(textureSize.y) / 2.0f});
			}  
		}
	}
}

void powe::SFMLSprite::SetOrigin(const glm::fvec2& pos)
{
	SetOrigin(pos.x, pos.y);
}

glm::fvec2 powe::SFMLSprite::GetOrigin() const
{
	if (const auto gameObject{ m_Owner.lock() })
	{
		if (SFMLSpriteComponent * sfmlSprite{ gameObject->GetComponent<SFMLSpriteComponent>() })
		{
			const sf::Vector2f& origin{ sfmlSprite->sprite.getOrigin() };
			return { origin.x,origin.y };
		}
	}

	return {};
}

void powe::SFMLSprite::SetRect(const glm::fvec4& rect)
{
	if (const auto gameObject{ m_Owner.lock() })
	{
		if (SFMLSpriteComponent * sfmlSprite{ gameObject->GetComponent<SFMLSpriteComponent>() })
		{
			sfmlSprite->sprite.setTextureRect(sf::IntRect{ {int(rect.x),int(rect.y)},{int(rect.z),int(rect.w)} });
			sfmlSprite->sprite.setOrigin({ rect.z / 2.0f,rect.w / 2.0f });
		}
	}
}

powe::SFMLSpriteComponent* powe::SFMLSprite::GetSfSprite() const
{
	const auto gameObject{ m_Owner.lock() };
	return gameObject ? nullptr : gameObject->GetComponent<SFMLSpriteComponent>();
}
