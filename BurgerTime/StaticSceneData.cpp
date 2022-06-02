#include "StaticSceneData.h"

#include "POWEngine/JSON/json.hpp"
#include "POWEngine/Logger/LoggerUtils.h"
#include <iostream>
#include <fstream>

void StaticSceneData::Initialize()
{
	// Parsing data file
	nlohmann::json json{};
	std::ifstream levelDataFile{ "./Resources/Level/LevelsProperties.json" };
	levelDataFile >> json;

	for (const auto& [key, value] : json.items())
	{
		int levelIndex{ std::stoi(key) };
		levelIndex;
		std::cout << value << '\n';
		for (const auto& item : value)
		{
			const auto& rect{ item["Rect"] };
			LevelData levelData{};
			levelData.spriteRect.x = rect["x"];
			levelData.spriteRect.y = rect["y"];
			levelData.spriteRect.z = rect["width"];
			levelData.spriteRect.w = rect["height"];

			const auto& playerSpawnPos{ item["PlayerSpawn"] };
			levelData.playerSpawnPoints.x = playerSpawnPos["x"];
			levelData.playerSpawnPoints.y = playerSpawnPos["y"];

			m_LevelData[levelIndex] = levelData;
		}
	}
}

LevelData StaticSceneData::GetLevelData(int levelIdx) const
{
	if (m_LevelData.contains(levelIdx))
		return m_LevelData.at(levelIdx);

	return {};
}
