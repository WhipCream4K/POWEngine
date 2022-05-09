#pragma once


class SoundAPI
{
public:

	SoundAPI() = default;
	virtual ~SoundAPI() = default;

public:

	virtual void Play(int soundClip) = 0;
};

