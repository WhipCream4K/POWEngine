#include "pch.h"
#include "AudioClip.h"

#include "FMODUtils.h"

powe::AudioClip::AudioClip(SoundID id,const std::string& filePath)
	: m_Id(id)
	, m_FilePath(filePath)
{
}

bool powe::AudioClip::IsLoaded() const
{
	return m_SoundInst;
}

void powe::AudioClip::LoadStream(FMOD::System* system)
{
	if(!m_SoundInst)
	{
		const auto result{ system->createStream(m_FilePath.c_str(),FMOD_2D | FMOD_CREATESTREAM | FMOD_LOOP_OFF,nullptr,&m_SoundInst) };
		FMODErrorCheck(result);
	}
}

void powe::AudioClip::LoadCompressed(FMOD::System* system)
{
	if(!m_SoundInst)
	{
		const auto result{ system->createSound(m_FilePath.c_str(),FMOD_2D | FMOD_CREATECOMPRESSEDSAMPLE | FMOD_LOOP_OFF,nullptr,&m_SoundInst) };
		FMODErrorCheck(result);
	}
}

powe::AudioClip::~AudioClip()
{
	if (m_SoundInst)
		m_SoundInst->release();
}
