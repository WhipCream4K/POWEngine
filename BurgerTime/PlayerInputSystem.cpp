#include "PlayerInputSystem.h"

//#include <powengine.h>
#include "BurgerTimeComponents.h"
#include "POWEngine/Core/Components/AudioComponent.h"

PlayerInputSystem::PlayerInputSystem()
{
	using namespace powe;

	DEFINE_SYSTEM_KEY(AudioComponent);
}

void PlayerInputSystem::OnUpdate(float, powe::GameObjectID)
{
	//POWLOGINFO("Hey");
	//InputComponent* inputTest{ GetComponent<InputComponent>() };
	//POWLOGINFO(std::to_string(inputTest->tag));
}

void PlayerInputSystem::OnCreate(powe::GameObjectID )
{
	using namespace powe;

	AudioComponent* audio{ GetComponent<AudioComponent>() };

	SoundInfo info{};
	info.volume = 1.0f;
	info.pitch = 1.0f;
	info.isLooped = false;

	audio->Play(info);
}
