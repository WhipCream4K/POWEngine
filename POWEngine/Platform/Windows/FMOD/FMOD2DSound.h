#pragma once

#include "Sound/SoundService.h"

namespace powe
{
	class FMOD2DSound final : public SoundService
	{
	public:

		FMOD2DSound(uint32_t nbChannels);
		FMOD2DSound(const FMOD2DSound&) = delete;
		FMOD2DSound& operator=(const FMOD2DSound&) = delete;
		FMOD2DSound(FMOD2DSound&&) = delete;
		FMOD2DSound& operator=(FMOD2DSound&&) = delete;
		~FMOD2DSound() override;

	public:

		SoundID RegisterSoundEntity(const std::string& filePath, bool shouldLoaded);
		void UnRegisterSoundEntity(SoundID id);
		bool IsPlaying(SoundID id) const;
		ChannelID Play(SoundID id, const SoundInfo& soundInfo);
		ChannelID PlayImmediate(const std::string& filePath, const SoundInfo& soundInfo);
		void Update();
		void Stop(SoundID id);

	private:

		class FMODSoundImpl;
		UniquePtr<FMODSoundImpl> m_FmodSoundImpl;
	};
}

 
