#include "pch.h"
#include "FMODAudioClip.h"

#include "FMODUtils.h"

powe::FMODAudioClip::FMODAudioClip(SoundID id, const std::string& filePath)
	: m_Id(id)
	, m_FilePath(filePath)
{
}

bool powe::FMODAudioClip::IsLoaded() const
{
	return m_SoundInst;
}

void powe::FMODAudioClip::LoadStream(FMOD::System* system)
{
	if (!m_SoundInst)
	{
		const auto result{ system->createStream(m_FilePath.c_str(),FMOD_2D | FMOD_CREATESTREAM | FMOD_LOOP_OFF,nullptr,&m_SoundInst) };
		FMODErrorCheck(result);
	}
}

void powe::FMODAudioClip::LoadCompressed(FMOD::System* system)
{
	if (!m_SoundInst)
	{
		const auto result{ system->createSound(m_FilePath.c_str(),FMOD_2D | FMOD_CREATECOMPRESSEDSAMPLE | FMOD_LOOP_OFF,nullptr,&m_SoundInst) };
		FMODErrorCheck(result);
	}
}

powe::FMODAudioClip::~FMODAudioClip()
{
	if (m_SoundInst)
		m_SoundInst->release();
}
