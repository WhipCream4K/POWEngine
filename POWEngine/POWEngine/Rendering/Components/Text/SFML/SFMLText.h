#pragma once

#include "POWEngine/Rendering/Components/Text/TextImpl.h"

namespace powe
{
	class Font;
	class GameObject;
	class SFMLText : public TextImpl
	{
	public:

		SFMLText() = default;
		SFMLText(int fontSize,const SharedPtr<GameObject>& gameObject);
		int GetTextSize() const override;
		void SetFont(const SharedPtr<Font>& font) override;
		void SetTextSize(int ptSize) override;
		void SetText(const std::string&) override;
		int GetRenderOrder() const override;
		void SetRenderOrder(int order) override;

	private:

		WeakPtr<GameObject> m_Owner;
		int m_TextSize;
	};
}


