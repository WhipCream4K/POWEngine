#pragma once

#include <poweCustomtype.h>

class GameState;
class PlayScene;
class SceneFactory;

namespace powe
{
	class GameObject;
	class WorldEntity;
	class WorldClock;
	class Core;
	class Texture;
}

class TestScene;
class DynamicSceneData;
class BurgerTimeGame
{
public:


	void Start(const SharedPtr<powe::Core>& core,const SharedPtr<powe::WorldEntity>& worldEntity);
	void Run(const SharedPtr<powe::WorldEntity>& worldEntity,const SharedPtr<powe::WorldClock>& worldClock);

private:


	SharedPtr<SceneFactory> m_PlayScene;
	SharedPtr<SceneFactory> m_MenuScene;

	SharedPtr<GameState> m_MainGameState;

	DynamicSceneData* m_SceneData{};

	powe::GameObjectID m_SceneDataID{};
	int m_Test{};

};

