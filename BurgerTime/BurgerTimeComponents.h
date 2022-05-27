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
