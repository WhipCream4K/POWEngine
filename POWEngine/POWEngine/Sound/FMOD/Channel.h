#pragma once

#include "POWEngine/Sound/SoundTypes.h"
#include "POWEngine/Sound/SoundInfo.h"

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

		bool IsPlaying() const;
		void Play(FMOD::System* system,FMOD::Sound* sound);
		void Stop() const;
		ChannelID GetID() const { return m_Id; }

	private:

		FMOD::Channel* m_ChannelInst{};
		ChannelID m_Id{};
		SoundInfo m_Info{};
		std::atomic_bool m_IsActive{};
	};
}



