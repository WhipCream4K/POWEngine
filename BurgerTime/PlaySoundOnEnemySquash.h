#pragma once

#include "SquashEnemyListener.h"
#include "POWEngine/Sound/SoundTypes.h"
#include "SoundPlayer.h"

class PlaySoundOnEnemySquash : public SquashEnemyListener , public SoundPlayer
{
public:

	PlaySoundOnEnemySquash() = default;
	void OnReceiveSquashEnemy(powe::WorldEntity&, EnemyType) override;
	void RegisterSound() override;
};

