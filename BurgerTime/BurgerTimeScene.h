#pragma once

#include <poweCustomtype.h>

namespace powe
{
	class GameObject;
	class WorldEntity;
	class WorldClock;
	class Core;
	class Texture;
}

class BurgerTimeScene
{
public:

	void Start(const SharedPtr<powe::Core>& core,const SharedPtr<powe::WorldEntity>& worldEntity);
	void Run(const SharedPtr<powe::WorldEntity>& worldEntity,const SharedPtr<powe::WorldClock>& worldClock);

private:

	SharedPtr<powe::Texture> m_MainTexture;
	std::vector<SharedPtr<powe::GameObject>> m_GameObjectNodes;

};

