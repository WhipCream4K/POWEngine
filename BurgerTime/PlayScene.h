#pragma once

#include <vector>
#include "poweCustomtype.h"
#include "SceneFactory.h"

struct PlayerDescriptor;

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

	SharedPtr<powe::GameObject> SpawnPlayer(powe::WorldEntity&,const PlayerDescriptor& desc);

	WeakPtr<powe::GameObject> m_SceneData{};
	powe::GameObjectID m_SceneDataID;
};

