#pragma once

#include <vector>
#include "poweCustomtype.h"
#include "SceneFactory.h"

namespace powe
{
	class WorldEntity;
	class GameObject;
}

class PlayScene : public SceneFactory, public std::enable_shared_from_this<PlayScene>
{
public:

	PlayScene(powe::GameObjectID sceneGameObject);

	void LoadScene(powe::WorldEntity&) override;

private:

	WeakPtr<powe::GameObject> m_SceneData{};
	powe::GameObjectID m_SceneDataID;
};

