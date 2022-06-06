#pragma once

#include "GameState.h"
#include "POWEngine/Math/Math.h"



class PlayScene;
class PlayState : public GameState
{
public:

	SharedPtr<GameState> HandleInput(powe::WorldEntity&, DynamicSceneData*, powe::GameObjectID) override;
	void Enter(powe::WorldEntity&, DynamicSceneData*, powe::GameObjectID) override;
	void Exit(powe::WorldEntity&, DynamicSceneData*, powe::GameObjectID) override;
	void Update(powe::WorldEntity&, float, DynamicSceneData*, powe::GameObjectID) override;

private:

	void EnableInput(powe::WorldEntity& worldEntity,DynamicSceneData* sceneData) const;

	//SharedPtr<PlayScene> m_PlayScene;
};

