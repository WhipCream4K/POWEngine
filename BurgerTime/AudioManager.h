#pragma once

#include <poweCustomtype.h>
#include <poweComponent.h>

#include "POWEngine/Sound/SoundTypes.h"
#include "BurgerEvent.h"
#include "POWEngine/Sound/SoundInfo.h"


class PlaySoundOnStep;

namespace powe
{
	struct SoundInfo;
}

enum class GameAudio
{
	Pepper,
};

class PlaySoundOnThrowPepper;
class PlaySoundOnEnemySquash;
class PlaySoundOnIngredientDrop;

enum class BasicSound
{
	BGM,
	ThrowPepper,
	EnemySquash,
	IngredientStep,
	IngredientSlam,
	GameStart,
	GameEnd,
	GameWin,

	Count
};


class PlaySoundOnEvent;
class AudioManager : public powe::Component<AudioManager>
{
public:

	AudioManager();

	void OnCreate(powe::WorldEntity&, powe::GameObjectID) override;

	void PlaySound(BasicSound type, const powe::SoundInfo& info);
	void PlaySound(BurgerEvent type);
	void StopSound(BasicSound type);

	SharedPtr<PlaySoundOnEvent> OnEventHappened{};
	std::unordered_map<BasicSound, powe::SoundID> m_BasicSounds;
	std::unordered_map<BasicSound, powe::SoundInfo> m_SoundInfos;
};

