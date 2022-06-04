#include "pch.h"
#include "AudioComponent.h"

#include "POWEngine/Services/ServiceLocator.h"
#include "POWEngine/Logger/LoggerUtils.h"

powe::AudioComponent::AudioComponent(const std::string& filePath, bool shouldPreloaded)
	: m_SoundID()
	, m_UsedChannel()
{
	m_SoundID = ServiceLocator::GetSoundSystem().RegisterSoundEntity(filePath, shouldPreloaded);
	//m_hey = std::make_unique<int>(42);
}

powe::AudioComponent::AudioComponent(const std::string& filePath, const SoundInfo& info, bool shouldPreloaded)
	: m_Info(info)
	, m_SoundID()
	, m_UsedChannel()
{
	m_SoundID = ServiceLocator::GetSoundSystem().RegisterSoundEntity(filePath, shouldPreloaded);
}


void powe::AudioComponent::OnDestroy(WorldEntity& , GameObjectID )
{
	ServiceLocator::GetSoundSystem().UnRegisterSoundEntity(m_SoundID);
}

void powe::AudioComponent::Play()
{
	m_UsedChannel = ServiceLocator::GetSoundSystem().Play(m_SoundID, m_Info);
}

void powe::AudioComponent::Play(const SoundInfo& info)
{
	m_UsedChannel = ServiceLocator::GetSoundSystem().Play(m_SoundID, info);
}

bool powe::AudioComponent::IsPlaying() const
{
	return ServiceLocator::GetSoundSystem().IsPlaying(m_UsedChannel);
}

void powe::AudioComponent::Stop() const
{
	ServiceLocator::GetSoundSystem().Stop(m_UsedChannel);
}
