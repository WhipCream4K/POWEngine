#pragma once

#include <poweCustomtype.h>

#include "PrefabDesc.h"

namespace powe
{
	class WorldEntity;
	class GameObject;
}

class HotDog
{
public:

	static SharedPtr<powe::GameObject> Create(powe::WorldEntity&,const EnemyDesc& desc);
};

