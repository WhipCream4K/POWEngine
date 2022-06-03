#pragma once

#include "POWEngine/Math/Math.h"
#include <poweCustomtype.h>

enum class PlayMode
{
	SinglePlayer,
	Coop,

	Count
};

namespace burger
{
	constexpr const char* MainObjectSprite{ "BurgerTime_MainSprite" };
	constexpr const char* MainLevelSprite{ "BurgerTime_MainLevelSprite" };
	constexpr const char* MenuPointer{ "BurgerTime_MenuPointer" };
	constexpr const char* MainFont{ "BurgerTime_MainFont" };
	constexpr glm::fvec2 SpriteScale{ 2.5f,2.5f };

	class RenderOrder
	{
	public:
		enum : uint8_t
		{
			Background,
			PickUps,
			Player,
			Enemy
		};
	};

	const glm::fvec2 MenuPos[int(PlayMode::Count)]
	{
		{650.0f,360.0f},
		{650.0f,400.0f}
	};
}

struct SpriteInfo
{
	glm::fvec4 rect{};
};

enum class IngredientsType
{
	TopBun,
	BottomBun,
	Mayo,
	Patty,
	Tomato,
	Lettuce
};



class OverlapLayer
{
public:

	enum : uint32_t
	{
		Player = (1 << 0),
		Ingredients = (1 << 1),
		Enemy = (1 << 2),

		Count = 3
	};

	static uint32_t LayerToIndex(uint32_t layer)
	{
		for (uint32_t i = 0; i < uint32_t(Count); ++i)
		{
			if ((1u << i) & layer)
				return i;
		}

		return uint32_t(Count);
	}
};





