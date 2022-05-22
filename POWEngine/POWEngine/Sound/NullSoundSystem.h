#pragma once
#include "POWEngine/Sound/SoundSystem.h"

namespace powe
{
	class NullSoundSystem final :
		public SoundSystem
	{
	public:

		NullSoundSystem() = default;
		NullSoundSystem(const NullSoundSystem&) = delete;
		NullSoundSystem& operator=(const NullSoundSystem&) = delete;
		NullSoundSystem(NullSoundSystem&&) = delete;
		NullSoundSystem& operator=(NullSoundSystem&&) = delete;
		~NullSoundSystem() override = default;

	public:

		SoundID RegisterSoundEntity(const std::string&, bool) override { return {}; }
		void UnRegisterSoundEntity(SoundID) override {}

		void Update() override {}

		// Play saved sound from memory
		ChannelID Play(SoundID , const SoundInfo& ) override { return {}; }

		//virtual SoundID PreLoadSoundToMemory(const std::string& filePath) = 0;
		//virtual void RemovePreLoadSound(SoundID id) = 0;

		// Play the sound immediately without saving to the memory
		ChannelID PlayImmediate(const std::string& , const SoundInfo& ) override { return {}; }
		bool IsPlaying(ChannelID ) const override { return false; }
		void Stop(ChannelID ) override {}

	};
}


