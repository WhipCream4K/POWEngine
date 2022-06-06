#include "pch.h"
#include "Channel.h"

#include "FMODUtils.h"

powe::Channel::Channel(ChannelID id, const SoundInfo& info)
	: m_Id(id)
	, m_Info(info)
{
}

bool powe::Channel::IsPlaying()
{
	if (!m_IsActive)
		return true;

	bool isPlaying{};

	{
		std::scoped_lock lock{ m_LocalChannelMutex };

		if (!m_ChannelInst)
			return false;

		FMODErrorCheck(m_ChannelInst->isPlaying(&isPlaying));
	}

	return isPlaying;
}

void powe::Channel::Play(FMOD::System* system, FMOD::Sound* sound)
{
	auto result{ system->playSound(sound, nullptr, true, &m_ChannelInst) };
	FMODErrorCheck(result);

	m_IsActive = true;
	m_IsPlaying = true;

	const int loopCount{ m_Info.isLooped ? -1 : 0 };

	{
		std::scoped_lock lock{ m_LocalChannelMutex };
		m_ChannelInst->setLoopCount(loopCount);
		m_ChannelInst->setVolume(m_Info.volume);
		m_ChannelInst->setPitch(m_Info.pitch);
		m_ChannelInst->setPaused(false);
	}

}

void powe::Channel::Stop()
{
	if (m_IsActive)
	{
		{
			std::scoped_lock lock{ m_LocalChannelMutex };
			FMODErrorCheck(m_ChannelInst->stop());
			m_ChannelInst = nullptr;
		}

		m_IsPlaying = false;
	}
}

powe::Channel::~Channel() = default;