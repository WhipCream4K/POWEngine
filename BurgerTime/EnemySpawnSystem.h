#pragma once

#include <poweSystem.h>

#include "WalkableTiles.h"
#include "POWEngine/Math/Math.h"

namespace powe
{
	class GameObject;
}

class EnemySpawnSystem : public powe::SystemBase
{
public:

	EnemySpawnSystem(int levelIdx);

protected:

	void OnUpdate(float, powe::GameObjectID) override;

	float m_SpawnTime;
	int m_MaxEnemy{ 3 };
	std::vector<SharedPtr<powe::GameObject>> m_SpawnedEnemy;
	std::vector<glm::ivec2> m_CurrentSpawnableTile{};
	std::vector<TileData> m_CurrentTileData{};
	int m_MaxColTile{};
	int m_MaxRowTile{};
};

