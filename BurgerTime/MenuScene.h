#pragma once

#include <poweCustomtype.h>
#include "SceneFactory.h"

class MenuScene : public SceneFactory
{
public:

	MenuScene(powe::GameObjectID);
	void LoadScene(powe::WorldEntity&) override;

private:

	powe::GameObjectID m_SceneDataID{};
};

