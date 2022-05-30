#pragma once

#include <poweCustomtype.h>

namespace powe
{
	class GameObject;
	class WorldEntity;
}

class Player
{
public:

	static SharedPtr<powe::GameObject> Create(powe::WorldEntity& worldEntity);

};

