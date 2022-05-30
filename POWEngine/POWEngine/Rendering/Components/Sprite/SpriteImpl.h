#pragma once

#include "POWEngine/ECS/ECSTypes.h"
#include "POWEngine/Rendering/2D/Draw2DObject.h"

namespace powe
{
	class Texture;
	class WorldEntity;
	class GameObject;
	class SpriteImpl : public Draw2DObject
	{
	public:

		SpriteImpl();
		SpriteImpl(const SharedPtr<GameObject>& gameObject);
		virtual void SetTexture(const Texture& texture) = 0;
		virtual void SetOrigin(float x, float y) = 0;
		virtual void SetOrigin(const glm::fvec2& pos) = 0;
		virtual glm::fvec2 GetOrigin() const = 0;
		virtual void SetRect(const glm::fvec4& rect) = 0;
		virtual ~SpriteImpl() override;
	};
}


