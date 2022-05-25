#pragma once

#include <vector>
#include "poweCustomtype.h"

namespace powe
{
	class WorldEntity;
	class GameObject;
}

class PlayScene
{
public:

	PlayScene();

	void Load(powe::WorldEntity& worldEntity);
	void UnLoad(powe::WorldEntity& worldEntity);

private:

	std::vector<SharedPtr<powe::GameObject>> m_GameObjects;
};

