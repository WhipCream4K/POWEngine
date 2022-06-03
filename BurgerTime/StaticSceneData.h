#pragma once

#include "WalkableTiles.h"
#include "POWEngine/Singleton/ThreadSafeSingleton.h"
#include "POWEngine/Math/Math.h"

struct LevelData
{
	glm::fvec4 spriteRect{};
	glm::fvec2 playerSpawnPoints{};
};

class StaticSceneData : public powe::ThreadSafeSingleton<StaticSceneData>
{
	friend class powe::ThreadSafeSingleton<StaticSceneData>;

public:

	void Initialize();

	LevelData GetLevelData(int levelIdx) const;
	TileData GetSingleTile(int levelIdx,int col, int row) const;
	const std::vector<TileData>& GetAllTileInLevel(int levelIdx) const;
	const glm::ivec2& GetPlayerStartTile(int levelIdx) const;
	int GetMaxColTile() const { return m_MaxColTile; }
	int GetMaxRowTile() const { return m_MaxRowTile; }
	

private:

	std::unordered_map<int, LevelData> m_LevelData;
	std::unordered_map<int, std::vector<TileData>> m_LevelTiles;
	std::unordered_map<int, glm::ivec2> m_PlayerStartTile;
	int m_MaxColTile{ 17 };
	int m_MaxRowTile{ 13 };
};

