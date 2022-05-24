#include "pch.h"
#include "SpriteComponent.h"

#include "POWEngine/Core/WorldEntity/WorldEntity.h"
//#include "SpriteImpl.h"
#include "POWEngine/Logger/LoggerUtils.h"

#if USE_SFML_RENDERER
#include "POWEngine/Rendering/Components/Sprite/SFML/SFMLSprite.h"
using SpriteType = powe::SFMLSprite;
#endif

powe::SpriteComponent::SpriteComponent()
	: m_SpriteImpl(std::make_shared<SpriteType>())
{
}

powe::SpriteComponent::SpriteComponent(WorldEntity* worldEntity, GameObjectID id)
	: m_SpriteImpl(std::make_shared<SpriteType>(worldEntity,id))
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

float powe::SpriteComponent::GetZDepth() const
{
	return m_ZDepth;
}

