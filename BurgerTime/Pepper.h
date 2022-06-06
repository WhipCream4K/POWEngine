#pragma once

#include <poweCustomtype.h>
#include "PrefabDesc.h"

namespace powe
{
	class WorldEntity;
	class GameObject;
}

class Pepper 
{
	static SharedPtr<powe::GameObject> Create(powe::WorldEntity& worldEntity, const PepperDesc& desc);
};

