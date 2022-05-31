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

	PlayScene(const SharedPtr<powe::GameObject>& dynamicScene);

	void LoadScene(powe::WorldEntity&) override;

private:

	WeakPtr<powe::GameObject> m_SceneData{};
	//powe::GameObjectID m_SceneDataID;
};

