#include "pch.h"
#include "SFMLDebugRectangle.h"

#include "POWEngine/Core/GameObject/GameObject.h"
#include "SFMLDebugRectangleComponent.h"

powe::SFMLDebugRectangle::SFMLDebugRectangle()
{
}

powe::SFMLDebugRectangle::SFMLDebugRectangle(const SharedPtr<GameObject>& owner)
	: m_Owner(owner)
{
	if(owner)
	{
		SFMLDebugRectangleComponent* debugRect = owner->AddComponent(SFMLDebugRectangleComponent{}, ComponentFlag::Sparse);
		debugRect->rectangle.setFillColor(sf::Color{ 255,255,255,0 });
		debugRect->rectangle.setOutlineThickness(1.5f);
		debugRect->rectangle.setOutlineColor({ 0,255,0,255 });
		constexpr sf::Vector2f defaultSize{ 20.0f,20.0f };
		debugRect->rectangle.setSize(defaultSize);
		debugRect->rectangle.setOrigin({ defaultSize.x / 2.0f,defaultSize.y / 2.0f });
		debugRect->drawOrder = 300;
	}
}

powe::SFMLDebugRectangle::SFMLDebugRectangle(const SharedPtr<GameObject>& owner, const glm::fvec2& size)
	: m_Owner(owner)
	, m_Size(size)
{
	if (owner)
	{
		SFMLDebugRectangleComponent* debugRect = owner->AddComponent(SFMLDebugRectangleComponent{}, ComponentFlag::Sparse);
		debugRect->rectangle.setFillColor(sf::Color{ 255,255,255,0 });
		debugRect->rectangle.setOutlineThickness(1.5f);
		debugRect->rectangle.setOutlineColor({ 0,255,0,255 });
		const sf::Vector2f defaultSize{ size.x,size.y };
		debugRect->rectangle.setSize(defaultSize);
		debugRect->rectangle.setOrigin({ size.x / 2.0f,size.y / 2.0f });
		debugRect->drawOrder = 300;
	}
}

void powe::SFMLDebugRectangle::SetOutlineThickness(float thickness)
{
	if (const auto gameObject{ m_Owner.lock() })
	{
		if (SFMLDebugRectangleComponent * sfmlRect{ gameObject->GetComponent<SFMLDebugRectangleComponent>() })
		{
			sfmlRect->rectangle.setOutlineThickness(thickness);
		}
	}
}

void powe::SFMLDebugRectangle::Destroy(powe::WorldEntity&, powe::GameObjectID)
{
	if (const auto gameObject{ m_Owner.lock() })
	{
		gameObject->RemoveComponent<SFMLDebugRectangleComponent>();
	}
}


void powe::SFMLDebugRectangle::SetFillColor(const glm::uvec4& color)
{
	if (const auto gameObject{ m_Owner.lock() })
	{
		if (SFMLDebugRectangleComponent * sfmlRect{ gameObject->GetComponent<SFMLDebugRectangleComponent>() })
		{
			const sf::Uint8 r{ uint8_t(color.x) };
			const sf::Uint8 g{ uint8_t(color.y) };
			const sf::Uint8 b{ uint8_t(color.z) };
			const sf::Uint8 a{ uint8_t(color.w) };

			sfmlRect->rectangle.setFillColor(sf::Color{ r,g,b,a });
		}
	}

	m_FillColor = color;
}
void powe::SFMLDebugRectangle::SetSize(const glm::fvec2& size)
{
	if (const auto gameObject{ m_Owner.lock() })
	{
		if (SFMLDebugRectangleComponent * sfmlRect{ gameObject->GetComponent<SFMLDebugRectangleComponent>() })
		{
			sfmlRect->rectangle.setSize({size.x,size.y});
			sfmlRect->rectangle.setOrigin({size.x / 2.0f,size.y / 2.0f});
		}
	}

	m_Size = size;
}

const glm::uvec4& powe::SFMLDebugRectangle::GetFillColor() const
{
	return m_FillColor;
}

const glm::uvec4& powe::SFMLDebugRectangle::GetOutlineColor() const
{
	return m_OutlineColor;
}

const glm::fvec2& powe::SFMLDebugRectangle::GetSize() const
{
	return m_Size;
}

int powe::SFMLDebugRectangle::GetRenderOrder() const
{
	if (const auto gameObject{ m_Owner.lock() })
	{
		if (SFMLDebugRectangleComponent * sfmlRect{ gameObject->GetComponent<SFMLDebugRectangleComponent>() })
		{
			return sfmlRect->drawOrder;
		}
	}

	return 0;
}

void powe::SFMLDebugRectangle::SetRenderOrder(int order)
{
	if (const auto gameObject{ m_Owner.lock() })
	{
		if (SFMLDebugRectangleComponent * sfmlRect{ gameObject->GetComponent<SFMLDebugRectangleComponent>() })
		{
			sfmlRect->drawOrder = order;
		}
	}
}

const glm::fvec2& powe::SFMLDebugRectangle::GetOrigin() const
{
	return m_Origin;
}

void powe::SFMLDebugRectangle::SetOrigin(const glm::fvec2& origin)
{
	if (const auto gameObject{ m_Owner.lock() })
	{
		if (SFMLDebugRectangleComponent * sfmlRect{ gameObject->GetComponent<SFMLDebugRectangleComponent>() })
		{
			sfmlRect->rectangle.setOrigin({origin.x,origin.y});
		}
	}

	m_Origin = origin;
}

void powe::SFMLDebugRectangle::SetOutlineColor(const glm::uvec4& color)
{
	if (const auto gameObject{ m_Owner.lock() })
	{
		if (SFMLDebugRectangleComponent * sfmlRect{ gameObject->GetComponent<SFMLDebugRectangleComponent>() })
		{
			const sf::Uint8 r{ uint8_t(color.x) };
			const sf::Uint8 g{ uint8_t(color.y) };
			const sf::Uint8 b{ uint8_t(color.z) };
			const sf::Uint8 a{ uint8_t(color.w) };

			sfmlRect->rectangle.setOutlineColor(sf::Color{ r,g,b,a });
		}
	}

	m_OutlineColor = color;
}
