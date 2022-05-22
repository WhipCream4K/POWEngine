#pragma once

#include "POWEngine/Core/Components/BaseComponent.h"

namespace powe
{
	class Texture;
	class SpriteImpl;
	class SpriteComponent : public Component<SpriteComponent>
	{
	public:

		SpriteComponent() = default;
		SpriteComponent(WorldEntity* worldEntity,GameObjectID id);
		void SetTexture(const Texture& texture) const;
		void SetRect(const glm::fvec4& rect) const;
		void SetOrigin(float x, float y) const;
		void SetOrigin(const glm::fvec2& pos) const;
		float GetZDepth() const;


	private:

		OwnedPtr<SpriteImpl> m_SpriteImpl;
		float m_ZDepth{};
	};
}


