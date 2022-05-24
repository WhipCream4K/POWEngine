#include "PlayerInputSystem.h"

#include "BurgerTimeComponents.h"
#include "POWEngine/Core/Components/AudioComponent.h"

PlayerInputSystem::PlayerInputSystem()
{
	using namespace powe;

	DEFINE_SYSTEM_KEY(AudioComponent);
}

void PlayerInputSystem::OnUpdate(float, powe::GameObjectID)
{

}

void PlayerInputSystem::OnCreate(powe::GameObjectID )
{
	using namespace powe;

	AudioComponent* audio{ GetComponent<AudioComponent>() };

	SoundInfo info{};
	info.volume = 1.0f;
	info.pitch = 0.5f;
	info.isLooped = false;

	audio->Play(info);

	//GetWorld()->RemoveComponentByType<AudioComponent>(id);
}
