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
	Cheese,
	Patty,
	Tomato,
	Lettuce
};


enum class EnemyType
{
	HotDog
};

static inline IngredientsType StringToIngredients(const std::string& string)
{
	if (string == "TopBun")
		return IngredientsType::TopBun;
	if (string == "BottomBun")
		return IngredientsType::BottomBun;
	if (string == "Cheese")
		return IngredientsType::Cheese;
	if (string == "Patty")
		return IngredientsType::Patty;
	if (string == "Tomato")
		return IngredientsType::Tomato;
	if (string == "Lettuce")
		return IngredientsType::Lettuce;

	return IngredientsType::TopBun;
}



class OverlapLayer
{
public:

	enum : uint32_t
	{
		Player = (1 << 0),
		Ingredients = (1 << 1),
		Dynamic = (1 << 2),
		Static = (1 << 4),
		Enemy = (1 << 3),

		Count = 5
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





