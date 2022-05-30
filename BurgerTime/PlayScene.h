#pragma once

#include <vector>
#include "poweCustomtype.h"
#include "SceneFactory.h"

namespace powe
{
	class WorldEntity;
	class GameObject;
}

class PlayScene : public SceneFactory
{
public:

	PlayScene();

	void LoadScene(powe::WorldEntity&) override;

private:

	std::vector<SharedPtr<powe::GameObject>> m_GameObjects;
};

