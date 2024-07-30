#pragma once

#include "POWEngine/Sound/SoundTypes.h"
#include "POWEngine/Sound/SoundInfo.h"
#include <mutex>

namespace FMOD
{
	class Channel;
	class System;
	class Sound;
}

namespace powe
{
	class Channel
	{
	public:

		explicit Channel(ChannelID id,const SoundInfo& info);

		bool IsPlaying();
		void Play(FMOD::System* system,FMOD::Sound* sound);
		void Stop();
		ChannelID GetID() const { return m_Id; }
		~Channel();

	private:

		std::mutex m_LocalChannelMutex;
		FMOD::Channel* m_ChannelInst{};
		ChannelID m_Id{};
		SoundInfo m_Info{};
		std::atomic_bool m_IsActive{};
		std::atomic_bool m_IsPlaying{};
	};
}



