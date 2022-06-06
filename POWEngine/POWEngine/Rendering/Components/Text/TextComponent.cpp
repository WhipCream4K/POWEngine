#include "pch.h"
#include "TextComponent.h"

#include "POWEngine/Core/GameObject/GameObject.h"
#include "POWEngine/Rendering/2D/Draw2DObject.h"

#ifdef USE_SFML_RENDERER
#include "SFML/SFMLText.h"
using TextType = powe::SFMLText;
#endif

powe::TextComponent::TextComponent()
	: m_TextImpl(std::make_unique<TextType>())
{
}

powe::TextComponent::TextComponent(int fontSize,const SharedPtr<GameObject>& owner)
	: m_TextImpl(std::make_unique<TextType>(fontSize,owner))
{
}

powe::TextComponent::TextComponent(TextComponent&&) noexcept = default;

powe::TextComponent& powe::TextComponent::operator=(TextComponent&&) noexcept = default;

powe::TextComponent::~TextComponent() = default;

void powe::TextComponent::SetFont(const SharedPtr<Font>& font) const
{
	m_TextImpl->SetFont(font);
}

int powe::TextComponent::GetTextSize() const
{
	return m_TextImpl->GetTextSize();
}

void powe::TextComponent::SetTextSize(int ptSize) const
{
	m_TextImpl->SetTextSize(ptSize);
}

void powe::TextComponent::SetText(const std::string& text) const
{
	m_TextImpl->SetText(text);
}

void powe::TextComponent::SetFillColor(const glm::uvec4& color) const
{
	m_TextImpl->SetFillColor(color);
}
