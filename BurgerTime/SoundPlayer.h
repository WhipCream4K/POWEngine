#pragma once

#include "POWEngine/Sound/SoundTypes.h"

class SoundPlayer
{
public:

	SoundPlayer() = default;
	virtual void RegisterSound() {}
	virtual ~SoundPlayer() = default;

protected:

	powe::SoundID m_SoundId{};
};

