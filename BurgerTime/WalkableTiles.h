#pragma once

#include <poweComponent.h>

#include "POWEngine/Math/Math.h"

enum class TileType
{
	None,
	Platform,
	Ladder,
	LadderStart
};

struct TileData
{
	// relative position of the tile from the center of the owner
	glm::fvec2 relativePos{};
	glm::fvec2 position{};
	glm::fvec2 size{};
	TileType tileType{};
};

class WalkableTiles : public powe::Component<WalkableTiles>
{
public:

	WalkableTiles() = default;


private:

	std::unordered_map<int, TileData> m_TileData{};
};

