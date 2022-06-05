#include "StaticSceneData.h"

//#include "POWEngine/JSON/json.hpp"
#include "POWEngine/Logger/LoggerUtils.h"
#include "StaticVariables.h"
#include <iostream>
#include <fstream>
#include <ranges>

//#include "WalkableTiles.h"

void StaticSceneData::Initialize()
{
	// Parsing data file
	nlohmann::json json{};
	const std::string levelFolder{ "./Resources/Level/" };
	const std::string levelFilePath{ "./Resources/Level/LevelsProperties.json" };
	std::ifstream levelDataFile{ levelFilePath };
	levelDataFile >> json;


	std::unordered_map<int, std::vector<TileData>> tileData{};

	// Initialize Level Data
	for (const auto& [key, value] : json.items())
	{
		int levelIndex{ std::stoi(key) };
		for (const auto& item : value)
		{
			const auto& rect{ item["Rect"] };
			LevelData levelData{};
			levelData.spriteRect.x = rect["x"];
			levelData.spriteRect.y = rect["y"];
			levelData.spriteRect.z = rect["width"];
			levelData.spriteRect.w = rect["height"];

			const auto& playerSpawnPos{ item["PlayerSpawnPos"] };
			levelData.playerSpawnPoints.x = playerSpawnPos["x"];
			levelData.playerSpawnPoints.y = playerSpawnPos["y"];

			m_LevelData[levelIndex] = levelData;

			const std::string tileFileName{ item["TileData"] };
			std::ifstream tileDataFile{ levelFolder + tileFileName };

			// Parsing tile csv file
			std::string line{};
			while (std::getline(tileDataFile, line))
			{
				TileData tile{};
				std::istringstream stringStream{ line };
				std::string field{};
				while (std::getline(stringStream, field, ','))
				{
					tile.tileType = TileType(std::stoi(field));

					tileData[levelIndex].emplace_back(tile);
				}
			}

			// Player Spawn Tile
			const auto& playerStartTile = item["PlayerSpawnTile"];
			m_PlayerStartTile[levelIndex].x = playerStartTile["col"];
			m_PlayerStartTile[levelIndex].y = playerStartTile["row"];

			// Ingredient Spawn Tile
			const auto& ingredientPos{ item["IngredientPos"] };

			for (const auto& [row, spawnData] : ingredientPos.items())
			{
				IngredientSpawn ingSpawn{};
				ingSpawn.row = std::stoi(row);

				for (const auto& data : spawnData)
				{
					ingSpawn.type = StringToIngredients(data["type"]);
					ingSpawn.col = data["col"];

					m_IngredientSpawnInfo[levelIndex].emplace_back(ingSpawn);
				}
			}

			ParsePlateSpawnInfo(item, levelIndex);
		}
	}

	// Initialize Relative tile position

	const int maxCol{ 17 };
	const int maxRow{ 13 };
	const glm::fvec2 boxColSize{ 12.0f,12.0f };
	const glm::fvec2 boxRowSize{ 16.0f,16.0f };

	glm::fvec2 startPos{ -floor(maxCol / 2.0f) * boxColSize.x,
		-floor(maxRow / 2.0f) * boxRowSize.x };


	const glm::fvec2 midScreen{ 640.0f,360.0f };

	for (auto& tile : tileData | std::views::values)
	{
		glm::fvec2 boxPos{ startPos };
		for (int row = 0; row < maxRow; ++row)
		{
			for (int col = 0; col < maxCol; ++col)
			{
				auto& currentTile{ tile[(row * maxCol) + col] };
				currentTile.relativePos = boxPos;

				// TODO: fix where it gets the position from the parent
				currentTile.position = boxPos * burger::SpriteScale + midScreen;

				currentTile.size = { boxColSize.x,
					boxRowSize.x };

				currentTile.size *= burger::SpriteScale;

				boxPos.x += boxColSize.x;
			}
			boxPos.y += boxRowSize.x;
			boxPos.x = startPos.x;
		}
	}

	m_LevelTiles = tileData;

	std::string outdebug{ "File loaded" };
	outdebug.append("-> " + levelFilePath);
	POWLOGNORMAL(outdebug);


	ParseIngredientSpriteInfo();
}

LevelData StaticSceneData::GetLevelData(int levelIdx) const
{
	if (m_LevelData.contains(levelIdx))
		return m_LevelData.at(levelIdx);

	return {};
}

TileData StaticSceneData::GetSingleTile(int levelIdx, int col, int row) const
{
	if (m_LevelTiles.contains(levelIdx))
	{
		assert(col < 17 && row < 13);
		return m_LevelTiles.at(levelIdx)[(row * m_MaxColTile) + col];
	}

	return {};
}

const std::vector<TileData>& StaticSceneData::GetAllTileInLevel(int levelIdx) const
{
	return m_LevelTiles.at(levelIdx);
}

const glm::ivec2& StaticSceneData::GetPlayerStartTile(int levelIdx) const
{
	return m_PlayerStartTile.at(levelIdx);
}

const SpriteInfo& StaticSceneData::GetIngredientSpriteInfo(IngredientsType type) const
{
	return m_IngredientSpriteInfo.at(type);
}

int StaticSceneData::GetPlateServingPiecesCount(int levelIdx) const
{
	return m_PlateServingCount.at(levelIdx);
}

const std::vector<IngredientSpawn>& StaticSceneData::GetIngredientSpawnInfo(int levelIdx) const
{
	return m_IngredientSpawnInfo.at(levelIdx);
}

const std::vector<PlateSpawn>& StaticSceneData::GetPlateSpawnInfo(int levelIdx) const
{
	return m_PlateSpawnInfo.at(levelIdx);
}

void StaticSceneData::ParseIngredientSpriteInfo()
{
	const std::string spriteFilePath{ "./Resources/Level/IngredientProperties.json" };
	std::ifstream ifstream{ spriteFilePath };

	nlohmann::json json{};

	ifstream >> json;

	for (const auto& [key, value] : json.items())
	{
		const IngredientsType type{ IngredientsType(std::stoi(key)) };

		for (const auto& item : value)
		{
			const auto& rect{ item["Rect"] };
			SpriteInfo info{};
			info.rect.x = rect["x"];
			info.rect.y = rect["y"];
			info.rect.z = rect["width"];
			info.rect.w = rect["height"];

			m_IngredientSpriteInfo.try_emplace(type, info);
		}
	}


	std::string outdebug{ "File loaded" };
	outdebug.append("-> " + spriteFilePath);
	POWLOGNORMAL(outdebug);
}

void StaticSceneData::ParsePlateSpawnInfo(const nlohmann::basic_json<>& item, int levelIdx)
{
	const auto& plateSpawnPos{ item["PlateSpawnPos"] };

	const glm::fvec2 spriteSize{ 32.0f,2.0f };

	// TODO: should have get the level position somewhere
	const glm::fvec2 midScreen{ 640.0f,360.0f };
	const glm::fvec2 levelSize{ 208.0f,208.0f };

	const glm::fvec2 refPoint{ midScreen - ( (levelSize / 2.0f) * burger::SpriteScale) };

	for (const auto& coor : plateSpawnPos)
	{
		PlateSpawn plate{};
		plate.position.x = coor["x"];
		plate.position.y = coor["y"];

		plate.size = spriteSize;

		plate.position = (plate.position * burger::SpriteScale) + refPoint;

		m_PlateSpawnInfo[levelIdx].emplace_back(plate);
	}

	const int servingCount{ item["NumberOnPlate"] };
	m_PlateServingCount[levelIdx] = servingCount;
}

