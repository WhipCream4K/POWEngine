#include "PlaySoundOnEnemySquash.h"

#include "POWEngine/Services/ServiceLocator.h"

void PlaySoundOnEnemySquash::OnReceiveSquashEnemy(powe::WorldEntity&, EnemyType)
{
	using namespace powe;
	auto& audioSystem{ powe::ServiceLocator::GetSoundSystem() };


	// No need to check if it's playing
	SoundInfo info{};
	info.pitch = 1.0f;
	info.volume = 0.7f;

	audioSystem.Play(m_SoundId, info);

}

void PlaySoundOnEnemySquash::RegisterSound()
{
	using namespace powe;
	const std::string enemySquashSound{ "./Resources/Sound/EnemySquash.mp3" };
	m_SoundId = ServiceLocator::GetSoundSystem().RegisterSoundEntity(enemySquashSound);
}
