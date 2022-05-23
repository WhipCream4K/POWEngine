#pragma once

//#include "POWEngine/Services/Service.h"
#include <future>
#include "SoundTypes.h"
#include "SoundInfo.h"

namespace powe
{

	class SoundSystem
	{
	public:

		SoundSystem() = default;
		SoundSystem(const SoundSystem&) = delete;
		SoundSystem& operator=(const SoundSystem&) = delete;
		SoundSystem(SoundSystem&&) = delete;
		SoundSystem& operator=(SoundSystem&&) = delete;
		virtual ~SoundSystem() = default;

	public:

		virtual SoundID RegisterSoundEntity(const std::string& filePath, bool shouldLoaded = false) = 0;
		virtual void UnRegisterSoundEntity(SoundID id) = 0;

		virtual void Update() = 0;

		// Play saved sound from memory
		virtual ChannelID Play(SoundID id, const SoundInfo& soundInfo) = 0;

		//virtual SoundID PreLoadSoundToMemory(const std::string& filePath) = 0;
		//virtual void RemovePreLoadSound(SoundID id) = 0;

		// Play the sound immediately without saving to the memory
		virtual ChannelID PlayImmediate(const std::string& filePath, const SoundInfo& soundInfo) = 0;
		virtual bool IsPlaying(SoundID id) const = 0;
		virtual void Stop(SoundID id) = 0;

	};
}



