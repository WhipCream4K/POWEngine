#pragma once

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

private:

	std::unordered_map<int, LevelData> m_LevelData;
};

