#pragma once

#include "WalkableTiles.h"
#include "POWEngine/Singleton/ThreadSafeSingleton.h"
#include "POWEngine/Math/Math.h"
#include "StaticVariables.h"
#include "POWEngine/JSON/json.hpp"

struct LevelData
{
	glm::fvec4 spriteRect{};
	glm::fvec2 playerSpawnPoints{};
};

struct IngredientSpawn
{
	IngredientsType type{};
	int row{};
	int col{};
};

struct PlateSpawn
{
	glm::fvec2 position{};
	glm::fvec2 size{};
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
	const SpriteInfo& GetIngredientSpriteInfo(IngredientsType type) const;

	int GetPlateServingPiecesCount(int levelIdx) const;
	const std::vector<IngredientSpawn>& GetIngredientSpawnInfo(int levelIdx) const;
	const std::vector<PlateSpawn>& GetPlateSpawnInfo(int levelIdx) const;
	

private:

	void ParseIngredientSpriteInfo();

	void ParsePlateSpawnInfo(const nlohmann::basic_json<>& item,int levelIdx);

	std::unordered_map<int, LevelData> m_LevelData;
	std::unordered_map<int, std::vector<TileData>> m_LevelTiles;
	std::unordered_map<int, glm::ivec2> m_PlayerStartTile;
	std::unordered_map<IngredientsType, SpriteInfo> m_IngredientSpriteInfo;
	std::unordered_map<int, std::vector<IngredientSpawn>> m_IngredientSpawnInfo;
	std::unordered_map<int, std::vector<PlateSpawn>> m_PlateSpawnInfo{};
	std::unordered_map<int, int> m_PlateServingCount;

	int m_MaxColTile{ 17 };
	int m_MaxRowTile{ 13 };
};

