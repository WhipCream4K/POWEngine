#include "AudioManager.h"
#include "PlaySoundOnEvent.h"
#include "POWEngine/Services/ServiceLocator.h"

AudioManager::AudioManager()
//: OnPepperThrow(std::make_shared<PlaySoundOnThrowPepper>())
//, OnEnemySquash(std::make_shared<PlaySoundOnEnemySquash>())
//, OnPlayerStep(std::make_shared<PlaySoundOnStep>())
//, OnIngredientDrop(std::make_shared<PlaySoundOnIngredientDrop>())
{
}

void AudioManager::OnCreate(powe::WorldEntity&, powe::GameObjectID id)
{
	using namespace powe;

	for (int i = 0; i < int(BasicSound::Count); ++i)
	{
		std::string filePath{};

		SoundInfo info{};
		info.pitch = 1.0f;
		info.volume = 0.7f;

		switch (BasicSound(i))
		{
		case BasicSound::BGM:				filePath = "./Resources/Sound/BGM.mp3";
											info.isLooped = true;
			break;

		case BasicSound::GameStart:			filePath = "./Resources/Sound/GameStart.mp3";	break;
		case BasicSound::GameEnd:			filePath = "./Resources/Sound/GameOver.mp3";	break;
		case BasicSound::GameWin:			filePath = "./Resources/Sound/Win.wav";			break;

		case BasicSound::ThrowPepper:		filePath = "./Resources/Sound/PepperThrow_NoHit2.wav";
											info.pitch = 3.5f;
			break;

		case BasicSound::EnemySquash:		filePath = "./Resources/Sound/EnemySquash.mp3"; break;
		case BasicSound::IngredientStep:	filePath = "./Resources/Sound/Step.wav";		break;
		case BasicSound::IngredientSlam:	filePath = "./Resources/Sound/Slam.mp3";		break;
		case BasicSound::Count: break;
		default:;
		}

		m_BasicSounds[BasicSound(i)] = ServiceLocator::GetSoundSystem().RegisterSoundEntity(filePath);
		m_SoundInfos[BasicSound(i)] = info;
	}

	OnEventHappened = std::make_shared<PlaySoundOnEvent>(id);

}

void AudioManager::PlaySound(BasicSound type, const powe::SoundInfo& info)
{
	using namespace powe;

	auto& audioSystem{ ServiceLocator::GetSoundSystem() };

	const SoundID toPlaySound{ m_BasicSounds[type] };

	if (!audioSystem.IsPlaying(toPlaySound))
	{
		audioSystem.Play(toPlaySound, info);
	}
}

void AudioManager::PlaySound(BurgerEvent type)
{
	BasicSound soundType{};
	switch (type)
	{
	case BurgerEvent::ThrowPepper:				soundType = BasicSound::ThrowPepper;		break;
	case BurgerEvent::EnemySquash:				soundType = BasicSound::EnemySquash;		break;
	case BurgerEvent::HotDogDead:				soundType = BasicSound::EnemySquash;		break;
	case BurgerEvent::IngredientDropToPlatform:	soundType = BasicSound::IngredientSlam;		break;
	case BurgerEvent::StepOnIngredients:		soundType = BasicSound::IngredientStep;		break;
	case BurgerEvent::PlateFull: break;
	default:;
	}

	const auto findItr{ m_BasicSounds.find(soundType) };
	if(findItr != m_BasicSounds.end())
	{
		const powe::SoundInfo info = m_SoundInfos[soundType];
		powe::ServiceLocator::GetSoundSystem().Play(findItr->second,info);
	}
}


void AudioManager::StopSound(BasicSound type)
{
	using namespace powe;
	const SoundID stopSound{ m_BasicSounds[type] };
	ServiceLocator::GetSoundSystem().Stop(stopSound);
}
