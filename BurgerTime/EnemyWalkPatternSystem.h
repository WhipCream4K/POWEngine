#pragma once

#include <poweSystem.h>

#include "WalkableTiles.h"


class EnemyWalkPatternSystem : public powe::SystemBase
{
public:

	EnemyWalkPatternSystem(int levelIdx);

protected:

	void OnUpdate(float, powe::GameObjectID) override;

	std::vector<TileData> m_CurrentTileData{};
	int m_MaxColTile{};
	int m_MaxRowTile{};
};

