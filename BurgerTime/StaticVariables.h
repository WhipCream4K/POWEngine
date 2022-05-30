#pragma once

#include "POWEngine/Math/Math.h"

namespace burger
{
	constexpr const char* MainObjectSprite{ "BurgerTime_MainSprite" };
	constexpr const char* MainLevelSprite{ "BurgerTime_MainLevelSprite" };
	constexpr const char* MainFont{ "BurgerTime_MainFont" };
	constexpr glm::fvec2 SpriteScale{ 2.5f,2.5f };

	class RenderOrder
	{
	public:
		enum : uint8_t
		{
			Background,
			Player,
			Enemy
		};
	};
}
