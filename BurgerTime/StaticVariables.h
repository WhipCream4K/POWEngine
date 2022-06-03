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

struct PlayerDescriptor
{
	glm::fvec2 spawnPos{};
	int currentLevelIdx{};
	uint8_t playerIndex{};
};




