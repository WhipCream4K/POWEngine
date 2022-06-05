#pragma once

#include <poweCustomtype.h>
#include <poweComponent.h>


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
class AudioManager : public powe::Component<AudioManager>
{
public:

	AudioManager();
	void OnCreate(powe::WorldEntity&, powe::GameObjectID) override;

	SharedPtr<PlaySoundOnThrowPepper> OnPepperThrow{};
	SharedPtr<PlaySoundOnEnemySquash> OnEnemySquash{};
};

