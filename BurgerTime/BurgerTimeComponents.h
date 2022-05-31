#pragma once

#include <poweComponent.h>

struct AnimationComponent : powe::Component<AnimationComponent>
{
	AnimationComponent() = default;

	AnimationComponent(int nbSprites,float timeSpan)
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

enum class PlayMode
{
	SinglePlayer
};

struct DynamicSceneData : powe::Component<DynamicSceneData>
{
	int currentLevel{};
	PlayMode currentPlayMode{};
};

struct SceneReference : powe::Component<SceneReference>
{
	powe::GameObjectID sceneID{};
};

struct CanWalkOnTile : powe::Component<CanWalkOnTile>
{
	powe::GameObjectID walkableTileID{};
	int currentCol{};
	int currentRow{};
};
