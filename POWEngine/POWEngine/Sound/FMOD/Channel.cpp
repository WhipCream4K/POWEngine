#include "pch.h"
#include "Channel.h"

#include "FMODUtils.h"

powe::Channel::Channel(ChannelID id,const SoundInfo& info)
	: m_Id(id)
	, m_Info(info)
{
}

bool powe::Channel::IsPlaying() const
{
	if(!m_IsActive)
		return true;

	bool isPlaying{};
	FMODErrorCheck(m_ChannelInst->isPlaying(&isPlaying));
	return isPlaying;
}

void powe::Channel::Play(FMOD::System* system,FMOD::Sound* sound)
{
	auto result{ system->playSound(sound, nullptr, true, &m_ChannelInst) };
	FMODErrorCheck(result);

	m_IsActive = true;
	const int loopCount{ m_Info.isLooped ? -1 : 0 };
	m_ChannelInst->setLoopCount(loopCount);
	m_ChannelInst->setVolume(m_Info.volume);
	m_ChannelInst->setPitch(m_Info.pitch);
	m_ChannelInst->setPaused(false);
}

void powe::Channel::Stop() const
{
	if (m_ChannelInst)
		FMODErrorCheck(m_ChannelInst->stop());
}
