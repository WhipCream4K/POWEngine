#pragma once

#include "POWEngine/Core/Components/BaseComponent.h"
#include "POWEngine/Math/Math.h"

namespace powe
{
	class GameObject;
	class RectangleImpl;
	class DebugRectangle : public Component<DebugRectangle>
	{
	public:

		DebugRectangle();
		DebugRectangle(const SharedPtr<GameObject>& owner);
		DebugRectangle(const SharedPtr<GameObject>& owner, const glm::fvec2& size);
		DebugRectangle(const DebugRectangle&) = delete;
		DebugRectangle& operator=(const DebugRectangle&) = delete;
		DebugRectangle(DebugRectangle&&) noexcept;
		DebugRectangle& operator=(DebugRectangle&&) noexcept;

		~DebugRectangle() override;

		void OnDestroy(WorldEntity&, GameObjectID) override;

		void SetSize(const glm::fvec2& size) const;
		const glm::fvec2& GetRectSize() const;
		void SetFillColor(const glm::uvec4& color) const;
		const glm::uvec4& GetOutlineColor() const;
		void SetOutlineColor(const glm::uvec4& color) const;
		const glm::uvec4& GetFillColor() const;
		void SetOrigin(const glm::fvec2& origin) const;
		const glm::fvec2& GetOrigin() const;
		void SetShouldRender(bool state) { m_ShouldRender = state; }
		bool GetShouldRender() const { return m_ShouldRender; }
		void SetOutlineThickness(float thickness) const;

	private:

		OwnedPtr<RectangleImpl> m_RectangleImpl;
		bool m_ShouldRender{ true };
	};
}
