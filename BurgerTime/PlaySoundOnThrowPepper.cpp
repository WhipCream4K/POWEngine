#include "PlaySoundOnThrowPepper.h"

#include "POWEngine/Services/ServiceLocator.h"

void PlaySoundOnThrowPepper::OnReceiveThrowPepper(powe::WorldEntity&)
{
	using namespace powe;

	SoundInfo info{};
	info.volume = 0.7f;
	info.pitch = 4.0f;
	auto& audioSystem{ ServiceLocator::GetSoundSystem() };

	if(!audioSystem.IsPlaying(m_SoundId))
	{
		audioSystem.Play(m_SoundId, info);
	}
}

void PlaySoundOnThrowPepper::RegisterSound()
{
	using namespace powe;

	const std::string enemySquashSound{ "./Resources/Sound/PepperThrow_NoHit2.wav" };
	m_SoundId = ServiceLocator::GetSoundSystem().RegisterSoundEntity(enemySquashSound);
}
