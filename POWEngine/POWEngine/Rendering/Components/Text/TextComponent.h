#pragma once

#include "POWEngine/Core/Components/BaseComponent.h"

namespace powe
{
	class Font;
	class GameObject;
	class TextImpl;
	class TextComponent : public Component<TextComponent>
	{
	public:

		TextComponent();
		TextComponent(int fontSize,const SharedPtr<GameObject>& owner);
		TextComponent(const TextComponent&) = delete;
		TextComponent& operator=(const TextComponent&) = delete;
		TextComponent(TextComponent&&) noexcept;
		TextComponent& operator=(TextComponent&&)  noexcept;
		~TextComponent() override;

		void SetFont(const SharedPtr<Font>& font) const;
		int GetTextSize() const;
		void SetTextSize(int ptSize) const;
		void SetText(const std::string& text) const;

	private:

		OwnedPtr<TextImpl> m_TextImpl;
	};

}


