#pragma once

#include <poweComponent.h>

#include "PlayModeObserver.h"
#include "PlayModeSubject.h"
#include "StaticVariables.h"

struct AnimationComponent : powe::Component<AnimationComponent>
{
	AnimationComponent() = default;

	AnimationComponent(int nbSprites, float timeSpan)
		: spritePerSec(timeSpan / float(nbSprites))
		, totalSprite(nbSprites)
	{
	}

	float spritePerSec{};
	float totalTime{};
	int totalSprite{};
	int currentSprite{};
};

struct Speed : powe::Component<Speed>
{
	Speed() = default;

	Speed(float inSpeed)
		: speed(inSpeed)
	{
	}

	float speed{};
};

struct MenuData : powe::Component<MenuData>
{
	MenuData()
		: playModeSubject(std::make_shared<PlayModeSubject>())
	{
	}

	SharedPtr<PlayModeSubject> playModeSubject{};
};

struct DynamicSceneData : powe::Component<DynamicSceneData>
{
	DynamicSceneData() = default;

	DynamicSceneData(powe::GameObjectID ownerID)
		: playModeObserver(std::make_shared<PlayModeObserver>(ownerID))
	{
	}

	int currentLevel{};
	PlayMode currentPlayMode{ PlayMode::SinglePlayer };

	SharedPtr<PlayModeObserver> playModeObserver{};
};

struct SceneReference : powe::Component<SceneReference>
{
	powe::GameObjectID sceneID{};
};

enum class MoveDir
{
	None,
	Left,
	Right,
	Up,
	Down
};

struct MovementDetails
{
	glm::fvec2 oldPos{};
	glm::fvec2 futurePos{};
	MoveDir currentMovementDir{ MoveDir::None };
};

struct LimitPlayArea : powe::Component<LimitPlayArea>
{
	glm::fvec4 rect{};
};

struct PlayerTag : powe::Component<PlayerTag>
{

};

struct DelayedMovement : powe::Component<DelayedMovement>
{
	float timeToReachNextSplit{};
	float timeCounter{};
	bool isMoving{};
};

struct FallingComponent : powe::Component<FallingComponent>
{
	FallingComponent() = default;

	FallingComponent(float inSpeed)
		: speed(inSpeed)
	{
	}

	float speed{};
};

struct CanWalkOnTile : powe::Component<CanWalkOnTile>
{
	CanWalkOnTile() = default;

	CanWalkOnTile(int levelIdx)
		: currentLevel(levelIdx)
	{
	}

	MovementDetails movementDetails{};
	int currentLevel{};
	int currentCol{};
	int currentRow{};
};
