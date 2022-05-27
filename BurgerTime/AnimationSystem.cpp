#include "AnimationSystem.h"

#include "BurgerTimeComponents.h"

#include "POWEngine/Rendering/Components/Sprite/SpriteComponent.h"

AnimationSystem::AnimationSystem()
{
	using namespace powe;

	DEFINE_SYSTEM_KEY(AnimationComponent, SpriteComponent);
}

void AnimationSystem::OnUpdate(float deltaTime, powe::GameObjectID)
{
	using namespace powe;

	const auto& [animation, sprite] = GetComponentsView<AnimationComponent, SpriteComponent>();

	if(animation->totalTime < animation->spritePerSec)
	{
		animation->totalTime += deltaTime;
	}
	else
	{
		animation->totalTime = 0.0f;
		animation->currentSprite = (animation->currentSprite + 1) % animation->totalSprite;
		sprite->SetRect({ float(animation->currentSprite) * 16.0f,0.0f,16.0f,16.0f });
	}
}
