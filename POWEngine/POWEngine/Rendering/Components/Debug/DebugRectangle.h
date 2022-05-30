#pragma once

#include "POWEngine/Core/Components/BaseComponent.h"

namespace powe
{
	class GameObject;
	class RectangleImpl;
	class DebugRectangle : public Component<DebugRectangle>
	{
	public:

		DebugRectangle();
		DebugRectangle(const SharedPtr<GameObject>& owner);
		DebugRectangle(const DebugRectangle&) = delete;
		DebugRectangle& operator=(const DebugRectangle&) = delete;
		DebugRectangle(DebugRectangle&&) noexcept;
		DebugRectangle& operator=(DebugRectangle&&) noexcept;

		~DebugRectangle() override;

		void SetSize(const glm::fvec2& size) const;
		const glm::fvec2& GetRectSize() const;
		void SetFillColor(const glm::uvec4& color) const;
		const glm::uvec4& GetOutlineColor() const;
		void SetOutlineColor(const glm::uvec4& color) const;
		const glm::uvec4& GetFillColor() const;
		void SetOrigin(const glm::fvec2& origin) const;
		const glm::fvec2& GetOrigin() const;

	private:

		OwnedPtr<RectangleImpl> m_RectangleImpl;
	};
}
