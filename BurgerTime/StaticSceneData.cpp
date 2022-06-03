#include "StaticSceneData.h"

#include "POWEngine/JSON/json.hpp"
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
		}
	}

	// Initialize Relative tile position

	const int maxCol{ 17 };
	const int maxRow{ 13 };
	const glm::fvec2 boxColSize{ 12.0f,12.0f };
	const glm::fvec2 boxRowSize{ 16.0f,16.0f };

	glm::fvec2 startPos{ -floor(maxCol / 2.0f) * boxColSize.x,
		-floor(maxRow / 2.0f) * boxRowSize.x };


	const glm::fvec2 midScreen{640.0f,360.0f};

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
					boxRowSize.x};

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
		assert(col > 17 && row > 13);
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
