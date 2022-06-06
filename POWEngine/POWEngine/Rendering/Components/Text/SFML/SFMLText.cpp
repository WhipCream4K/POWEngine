#include "pch.h"
#include "SFMLText.h"

#include "POWEngine/Rendering/Resources/Font/Font.h"
#include "POWEngine/Core/GameObject/GameObject.h"
#include "SFMLTextComponent.h"
#include "POWEngine/Rendering/Resources/Font/SFML/SFMLFont.h"

powe::SFMLText::SFMLText(int fontSize, const SharedPtr<GameObject>& gameObject)
	: m_Owner(gameObject)
	, m_TextSize(fontSize)
{
	if(gameObject)
	{
		SFMLTextComponent* realData = gameObject->AddComponent(SFMLTextComponent{}, ComponentFlag::Sparse);
		if(realData)
			realData->text.setCharacterSize(fontSize);
	}
}

int powe::SFMLText::GetTextSize() const
{
	return m_TextSize;
}

void powe::SFMLText::SetFont(const SharedPtr<Font>& font)
{
	if (!font)
		return;

	if (const auto gameObject{ m_Owner.lock() })
	{
		SFMLTextComponent* sfText{ gameObject->GetComponent<SFMLTextComponent>() };
		if (!sfText)
			return;

		SFMLFont* sfFont{ font->GetTypeInstance<SFMLFont>() };
		if (sfFont)
		{
			sfText->text.setFont(sfFont->GetSFFont());
		}
	}
}

void powe::SFMLText::SetTextSize(int ptSize)
{
	if (const auto gameObject{ m_Owner.lock() })
	{
		SFMLTextComponent* sfText{ gameObject->GetComponent<SFMLTextComponent>() };
		if (!sfText)
			return;

		sfText->text.setCharacterSize(ptSize);
	}

	m_TextSize = ptSize;

}

void powe::SFMLText::SetText(const std::string& text)
{
	if (const auto gameObject{ m_Owner.lock() })
	{
		SFMLTextComponent* sfText{ gameObject->GetComponent<SFMLTextComponent>() };
		if (!sfText)
			return;

		sfText->text.setString(text);
	}
}

int powe::SFMLText::GetRenderOrder() const
{
	if (const auto gameObject{ m_Owner.lock() })
	{
		SFMLTextComponent* sfText{ gameObject->GetComponent<SFMLTextComponent>() };
		if (!sfText)
			return 0;

		return sfText->renderOrder;
	}

	return 0;
}

void powe::SFMLText::SetRenderOrder(int order)
{
	if (const auto gameObject{ m_Owner.lock() })
	{
		SFMLTextComponent* sfText{ gameObject->GetComponent<SFMLTextComponent>() };
		if (!sfText)
			return;

		sfText->renderOrder = order;
	}
}

void powe::SFMLText::SetFillColor(const glm::uvec4& color)
{
	if (const auto gameObject{ m_Owner.lock() })
	{
		SFMLTextComponent* sfText{ gameObject->GetComponent<SFMLTextComponent>() };
		if (!sfText)
			return;

		const sf::Uint8 r{ uint8_t(color.x) };
		const sf::Uint8 g{ uint8_t(color.y) };
		const sf::Uint8 b{ uint8_t(color.z) };
		const sf::Uint8 a{ uint8_t(color.w) };

		sfText->text.setFillColor(sf::Color{ r,g,b,a });
	}
}
