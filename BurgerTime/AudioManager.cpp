#include "AudioManager.h"

#include "PlaySoundOnEnemySquash.h"
#include "PlaySoundOnThrowPepper.h"

AudioManager::AudioManager()
	: OnPepperThrow(std::make_shared<PlaySoundOnThrowPepper>())
{
}

void AudioManager::OnCreate(powe::WorldEntity&, powe::GameObjectID)
{
	if(OnPepperThrow)
	{
		OnPepperThrow->RegisterSound();
		OnEnemySquash->RegisterSound();
	}
}
