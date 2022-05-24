#pragma once

#include <poweCustomtype.h>

namespace powe
{
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
	//SharedPtr<powe::Core> m_EngineCore{};
	//SharedPtr<powe::Window> m_Window{};
	//SharedPtr<powe::WorldEntity> m_WorldEntity{};
	//SharedPtr<powe::Renderer> m_MainRenderer{};
};

