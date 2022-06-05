#pragma once

#include "PepperThrowListener.h"
#include "SoundPlayer.h"


class PlaySoundOnThrowPepper : public PepperThrowListener,public SoundPlayer
{
public:

	PlaySoundOnThrowPepper() = default;
	void OnReceiveThrowPepper(powe::WorldEntity& worldEntity) override;
	void RegisterSound() override;
	~PlaySoundOnThrowPepper() override = default;
};

