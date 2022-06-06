#include "PlaySoundOnEvent.h"

#include "AudioManager.h"
#include "POWEngine/Core/WorldEntity/WorldEntity.h"

PlaySoundOnEvent::PlaySoundOnEvent(powe::GameObjectID audioManager)
	: m_AudioManager(audioManager)
{
}

void PlaySoundOnEvent::OnReceiveMessage(powe::WorldEntity& worldEntity, BurgerEvent burgerEvent)
{
	AudioManager* audioManager{ worldEntity.GetComponent<AudioManager>(m_AudioManager) };
	if (audioManager)
	{
		audioManager->PlaySound(burgerEvent);
	}
}

void PlaySoundOnEvent::OnReceiveMessageByPlayer(powe::WorldEntity& worldEntity, BurgerEvent burgerEvent, int)
{

	AudioManager* audioManager{ worldEntity.GetComponent<AudioManager>(m_AudioManager) };
	if(audioManager)
	{
		audioManager->PlaySound(burgerEvent);
	}
}
