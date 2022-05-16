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

		bool IsPlaying(SoundID id) override { return false; }

		SoundID RegisterSoundEntity(const std::string&, bool = false) override { return {}; }
		void UnRegisterSoundEntity(SoundID ) override {}
		void Update() override {}

		// Play saved sound from memory
		ChannelID Play(SoundID, const SoundInfo&) override { return {}; }

		SoundID PreLoadSoundToMemory(const std::string&) override { return {}; }
		void RemovePreLoadSound(SoundID ) override {}

		// Play the sound immediately without saving to the memory
		SoundID PlayImmediate(const std::string&, const SoundInfo&) override { return {}; }
		void Stop(ChannelID) override{}

	};
}


