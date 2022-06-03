#pragma once

#include "POWEngine/Rendering/Components/Debug/RectangleImpl.h"

namespace powe
{
	class SFMLDebugRectangle : public RectangleImpl
	{
	public:

		SFMLDebugRectangle();
		SFMLDebugRectangle(const SharedPtr<GameObject>& owner);
		SFMLDebugRectangle(const SharedPtr<GameObject>& owner, const glm::fvec2& size);
		void SetFillColor(const glm::uvec4& color) override;
		void SetSize(const glm::fvec2& size) override;
		const glm::uvec4& GetFillColor() const override;
		const glm::uvec4& GetOutlineColor() const override;
		const glm::fvec2& GetSize() const override;
		int GetRenderOrder() const override;
		void SetRenderOrder(int order) override;
		const glm::fvec2& GetOrigin() const override;
		void SetOrigin(const glm::fvec2& origin) override;
		void SetOutlineColor(const glm::uvec4& color) override;
		void SetOutlineThickness(float thickness) override;

	private:

		WeakPtr<GameObject> m_Owner{};
		glm::uvec4 m_FillColor{};
		glm::uvec4 m_OutlineColor{};
		glm::fvec2 m_Size{};
		glm::fvec2 m_Origin{};
	};
}

