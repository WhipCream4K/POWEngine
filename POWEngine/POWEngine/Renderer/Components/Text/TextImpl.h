#pragma once
// #include "POWEngine/Rendering/2D/Draw2DObject.h"

namespace powe
{
	class Font;
	class GameObject;

	class TextImpl
	{
	public:

		TextImpl() = default;
		TextImpl(int fontSize,const SharedPtr<GameObject>& owner);
		virtual void SetFont(const SharedPtr<Font>& font) = 0;
		virtual int GetTextSize() const = 0;
		virtual void SetTextSize(int ptSize) = 0;
		virtual void SetText(const std::string&) = 0;
		virtual void SetFillColor(const glm::uvec4& color) = 0;
		virtual int GetRenderOrder() const = 0;
		virtual void SetRenderOrder(int order) = 0;
		virtual ~TextImpl() = default;
	};
}


