#include "pch.h"
#include "SpriteComponent.h"

#include "POWEngine/Core/WorldEntity/WorldEntity.h"
#include "POWEngine/Logger/LoggerUtils.h"
#include "POWEngine/Core/GameObject/GameObject.h"

#if USE_SFML_RENDERER
#include "POWEngine/Rendering/Components/Sprite/SFML/SFMLSprite.h"
using SpriteType = powe::SFMLSprite;
#endif

powe::SpriteComponent::SpriteComponent()
	: m_SpriteImpl(std::make_unique<SpriteType>())
{
}

powe::SpriteComponent::SpriteComponent(const SharedPtr<GameObject>& gameObject)
	: m_SpriteImpl(std::make_unique<SpriteType>(gameObject))
	, m_Owner(gameObject)
{
}

powe::SpriteComponent::SpriteComponent(SpriteComponent&&) noexcept = default;

powe::SpriteComponent& powe::SpriteComponent::operator=(SpriteComponent&&) noexcept = default;

powe::SpriteComponent::~SpriteComponent() = default;

void powe::SpriteComponent::SetTexture(const Texture& texture) const
{
	m_SpriteImpl->SetTexture(texture);
}

void powe::SpriteComponent::SetRect(const glm::fvec4& rect) const
{
	m_SpriteImpl->SetRect(rect);
}

void powe::SpriteComponent::SetOrigin(float x, float y) const
{
	m_SpriteImpl->SetOrigin(x, y);
}

void powe::SpriteComponent::SetOrigin(const glm::fvec2& pos) const
{
	m_SpriteImpl->SetOrigin(pos);
}

void powe::SpriteComponent::SetRenderOrder(int order) const
{
	m_SpriteImpl->SetRenderOrder(order);
}

int powe::SpriteComponent::GetRenderOrder() const
{
	return m_SpriteImpl->GetRenderOrder();
}

glm::fvec2 powe::SpriteComponent::GetOrigin() const
{
	return m_SpriteImpl->GetOrigin();
}


