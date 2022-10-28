#pragma once

#include "POWEngine/ECS/ECSTypes.h"

namespace powe
{
	class Texture;
	class WorldEntity;
	class GameObject;
	class SpriteImpl
	{
	public:

		SpriteImpl();
		SpriteImpl(const SharedPtr<GameObject>& gameObject);
		virtual void SetTexture(const Texture& texture) = 0;
		virtual void SetOrigin(float x, float y) = 0;
		virtual void SetOrigin(const glm::fvec2& pos) = 0;
		virtual glm::fvec2 GetOrigin() const = 0;
		virtual void SetRect(const glm::fvec4& rect) = 0;
		virtual void Destroy(powe::WorldEntity&) = 0;
		virtual void SetTint(const glm::uvec4&) = 0;
		virtual int GetRenderOrder() const = 0;
		virtual void SetRenderOrder(int order) = 0;
		virtual ~SpriteImpl();
	};
}

