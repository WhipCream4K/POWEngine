#pragma once

#include <poweComponent.h>

#include "POWEngine/Math/Math.h"

struct TileData
{
	glm::fvec2 position{};
	glm::fvec2 size{};
	bool isWalkable{};
};

class WalkableTiles : public powe::Component<WalkableTiles>
{
public:

	WalkableTiles() = default;

	bool IsWalkable(int index) const;

private:

	std::unordered_map<int, TileData> m_TileData{};
};

