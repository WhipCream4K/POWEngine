#pragma once

#include "POWEngine/Rendering/2D/Draw2DObject.h"

namespace powe
{
	class GameObject;

	class RectangleImpl : public Draw2DObject
	{
	public:

		RectangleImpl() = default;
		RectangleImpl(const SharedPtr<GameObject>& owner);
		virtual void SetSize(const glm::fvec2& size) = 0;
		virtual const glm::fvec2& GetSize() const = 0;
		virtual void SetFillColor(const glm::uvec4& color) = 0;
		virtual const glm::uvec4& GetOutlineColor() const = 0;
		virtual void SetOutlineColor(const glm::uvec4& color) = 0;
		virtual const glm::uvec4& GetFillColor() const = 0;
		virtual void SetOrigin(const glm::fvec2& origin) = 0;
		virtual const glm::fvec2& GetOrigin() const = 0;
		virtual void SetOutlineThickness(float thickness) = 0;
		virtual ~RectangleImpl() override = default;
	};
}

