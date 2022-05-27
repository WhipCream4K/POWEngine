#pragma once

#include "POWEngine/Core/CustomTypes.h"
#include "SceneFactory.h"

namespace powe
{
	class SystemBase;
	class GameObject;
	class WorldEntity;
}

class TestScene : public SceneFactory
{
public:

	void LoadScene(powe::WorldEntity&) override;

};

