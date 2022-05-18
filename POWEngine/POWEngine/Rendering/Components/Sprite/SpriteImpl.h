#pragma once

#include "POWEngine/ECS/ECSTypes.h"

namespace powe
{
	class Texture;
	class WorldEntity;
	class SpriteImpl
	{
	public:

		SpriteImpl() = default;
		SpriteImpl(WorldEntity* worldEntity, GameObjectID id);
		virtual void SetTexture(const Texture& texture) = 0;
		virtual void SetOrigin(float x, float y) = 0;
		virtual void SetOrigin(const glm::fvec2& pos) = 0;
		virtual void SetRect(const glm::fvec4& rect) = 0;
		virtual ~SpriteImpl() = default;
	};
}


