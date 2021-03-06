#pragma once

#include <poweCustomtype.h>
#include "POWEngine/Math/Math.h"
#include "StaticVariables.h"

struct PlayerDescriptor
{
	glm::fvec2 spawnPos{};
	int currentLevelIdx{};
	uint8_t playerIndex{};
	powe::GameObjectID colliderManager{};
	glm::uvec4 tint{255,255,255,255};
};

namespace powe
{
	class GameObject;
	class WorldEntity;
}

class Player
{
public:

	static SharedPtr<powe::GameObject> Create(powe::WorldEntity& worldEntity,
		const PlayerDescriptor& playerDescriptor);

};

