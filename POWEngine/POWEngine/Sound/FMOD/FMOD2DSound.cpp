#include "pch.h"
#include "FMOD2DSound.h"

#include <fmod.hpp>
#include <fmod_errors.h>

#include <future>
#include <queue>
#include "POWEngine/LockFree/LFQueue.h"
#include "AudioClip.h"
#include "POWEngine/Logger/LoggerUtils.h"
#include "Channel.h"

class powe::FMOD2DSound::FMODSoundImpl
{
public:

	FMODSoundImpl(uint32_t nbChannels)
		: m_FMODSystem()
		, m_MaxChannelsCnt(nbChannels)
	{
		FMOD::System* system{};

		auto result{ FMOD::System_Create(&system) };
		if (result != FMOD_OK)
			throw std::runtime_error(FMOD_ErrorString(result));

		result = system->init(int(m_MaxChannelsCnt), FMOD_INIT_NORMAL, nullptr);

		if (result != FMOD_OK)
			throw std::runtime_error(FMOD_ErrorString(result));

		m_FMODSystem = system;

		InitializeThread();
	}

	~FMODSoundImpl();

	SoundID RegisterSoundEntity(const std::string& filePath, bool shouldPreloaded);
	void UnRegisterSoundEntity(SoundID id);
	ChannelID Play(SoundID id, const SoundInfo& info);
	ChannelID PlayImmediate(const std::string& filePath, const SoundInfo& soundInfo);
	void Stop(SoundID id);
	bool IsPlaying(SoundID id);
	//void Update();


private:

	void InitializeThread();
	void RunMainPlaySound();
	void RunChannelChecking();

	FMOD::System* m_FMODSystem{};
	uint32_t m_MaxChannelsCnt{};

	// Audio
	std::mutex m_AudioMapMutex{};
	std::unordered_map<SoundID, SharedPtr<AudioClip>> m_AudioClip{};
	std::unordered_map<std::string, SoundID> m_PathCheck{};
	std::atomic<SoundID> m_AudioClipCnt{};

	// Channels
	std::mutex m_ChannelMutex{};
	std::unordered_map<SoundID, SharedPtr<Channel>> m_ActiveChannels{};
	std::atomic<ChannelID> m_ChannelCnt{};

	std::future<void> m_SoundThread{};
	std::condition_variable m_MainSoundThreadCV{};
	bool m_IsThreadExit{};

	struct SoundQueue
	{
		SoundQueue(const SharedPtr<AudioClip>& clip, Channel* channel)
			: aClip(clip)
			, aChannel(channel)
		{
		}

		SoundQueue() = default;

		SharedPtr<AudioClip> aClip{};
		Channel* aChannel{};
	};

	// SoundQueue
	std::mutex m_SoundQueueMutex{};
	LFQueue<SoundQueue> m_SoundQueue;

	// Channel Integrity
	std::future<void> m_ChannelIntegCheck{};
};

powe::FMOD2DSound::FMODSoundImpl::~FMODSoundImpl()
{
	m_IsThreadExit = true;

	m_SoundQueue.Push(SoundQueue{});
	m_MainSoundThreadCV.notify_one();

	// AudioClip contains fmod sound file and it need to be clean before fmod system exited
	m_AudioClip.clear();

	m_FMODSystem->release();
	m_FMODSystem = nullptr;
}

powe::SoundID powe::FMOD2DSound::FMODSoundImpl::RegisterSoundEntity(const std::string& filePath, bool shouldPreloaded)
{
	if (!m_PathCheck.contains(filePath))
	{
		const SoundID soundID{ m_AudioClipCnt++ };
		const SharedPtr<AudioClip> clip{ std::make_shared<AudioClip>(soundID,filePath) };

		if (shouldPreloaded)
			clip->LoadCompressed(m_FMODSystem);

		m_PathCheck.try_emplace(filePath, soundID);

		{
			std::scoped_lock lock{ m_AudioMapMutex };
			m_AudioClip.try_emplace(soundID, clip);
		}

		return soundID;
	}

	return m_PathCheck[filePath];
}

void powe::FMOD2DSound::FMODSoundImpl::UnRegisterSoundEntity(SoundID id)
{
	SharedPtr<AudioClip> clip{};

	{
		std::scoped_lock lock{ m_AudioMapMutex };
		clip = m_AudioClip[id];
	}

	if (clip)
	{
		m_PathCheck.erase(clip->GetFilePath());

		{
			std::scoped_lock lock{ m_AudioMapMutex };
			m_ActiveChannels.erase(id);
		}
	}
}

void powe::FMOD2DSound::FMODSoundImpl::InitializeThread()
{
	m_SoundThread = std::async(std::launch::async, &FMODSoundImpl::RunMainPlaySound, this);
	m_ChannelIntegCheck = std::async(std::launch::async, &FMODSoundImpl::RunChannelChecking, this);
}

powe::ChannelID powe::FMOD2DSound::FMODSoundImpl::Play(SoundID id, const SoundInfo& info)
{
	SharedPtr<AudioClip> clip{};

	{
		std::scoped_lock lock{ m_AudioMapMutex };

		if (!m_AudioClip.contains(id))
			return {};

		clip = m_AudioClip[id];
	}

	SharedPtr<Channel> channel{};

	{
		std::scoped_lock lock{ m_ChannelMutex };
		channel = m_ActiveChannels[id];
	}

	if (!channel)
	{
		channel = std::make_shared<Channel>(m_ChannelCnt++, info);

		{
			std::scoped_lock lock{ m_ChannelMutex };
			m_ActiveChannels[id] = channel;
		}
	}


	const SoundQueue soundQueue{ clip,channel.get() };
	m_SoundQueue.Push(soundQueue);

	m_MainSoundThreadCV.notify_one();

	return channel->GetID();
}

powe::ChannelID powe::FMOD2DSound::FMODSoundImpl::PlayImmediate(const std::string& filePath, const SoundInfo& info)
{
	const SoundID soundID{ m_AudioClipCnt++ };
	const SharedPtr<AudioClip> audioClip{ std::make_shared<AudioClip>(soundID,filePath) };
	SharedPtr<Channel> channel{};

	{
		std::scoped_lock lock{ m_ChannelMutex };
		channel = m_ActiveChannels[soundID];
	}

	if (!channel)
	{
		channel = std::make_shared<Channel>(m_ChannelCnt++, info);

		{
			std::scoped_lock lock{ m_ChannelMutex };
			m_ActiveChannels[soundID] = channel;
		}
	}

	const SoundQueue soundQueue{ audioClip,channel.get() };
	m_SoundQueue.Push(soundQueue);

	m_MainSoundThreadCV.notify_one();

	return channel->GetID();
}

void powe::FMOD2DSound::FMODSoundImpl::Stop(SoundID id)
{
	SharedPtr<Channel> targetChannel{};

	{
		std::scoped_lock lock{ m_ChannelMutex };
		targetChannel = m_ActiveChannels[id];
		const auto findItr{ m_ActiveChannels.find(id) };
		if (findItr != m_ActiveChannels.end())
			targetChannel = findItr->second;
	}

	if (targetChannel)
		targetChannel->Stop();
}

bool powe::FMOD2DSound::FMODSoundImpl::IsPlaying(SoundID id)
{
	SharedPtr<Channel> targetChannel{};

	{
		std::scoped_lock lock{ m_ChannelMutex };
		const auto findItr{ m_ActiveChannels.find(id) };
		if (findItr != m_ActiveChannels.end())
			targetChannel = findItr->second;
	}

	if (targetChannel)
		return targetChannel->IsPlaying();


	return false;
}

//void powe::FMOD2DSound::FMODSoundImpl::Update()
//{
//	while (!m_IsThreadExit)
//	{
//		std::this_thread::sleep_for(std::chrono::milliseconds(17));
//
//		{
//			std::scoped_lock lock{ m_ChannelMutex };
//
//			std::erase_if(m_ActiveChannels, [](const auto& item)
//				{
//					const auto& [key, value] = item;
//					return !value || !value->IsPlaying();
//				});
//		}
//	}
//}


void powe::FMOD2DSound::FMODSoundImpl::RunMainPlaySound()
{
	// this is single a producer so we don't need atomic bool check
	while (!m_IsThreadExit)
	{
		{
			std::unique_lock lock{ m_SoundQueueMutex };

			m_MainSoundThreadCV.wait(lock, [this]()
				{
					return !m_SoundQueue.Empty();
				});
		}

		const SoundQueue element{ m_SoundQueue.Front() };
		m_SoundQueue.Pop();

		if (element.aChannel && element.aClip)
		{
			try
			{
				if (!element.aClip->IsLoaded())
					element.aClip->LoadStream(m_FMODSystem);

				element.aChannel->Play(m_FMODSystem, element.aClip->GetSound());

			}
			catch (const std::exception& e)
			{
				POWLOGERROR(e.what());
			}
		}
	}
}

void powe::FMOD2DSound::FMODSoundImpl::RunChannelChecking()
{
	while (!m_IsThreadExit)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(400));

		size_t eraseCount{};
		{
			std::scoped_lock lock{ m_ChannelMutex };

			eraseCount = std::erase_if(m_ActiveChannels, [](const auto& item)
				{
					const auto& [key, value] = item;
					return !value || !value->IsPlaying();
				});

		}

		if (eraseCount > 0)
		{
			std::string name{};
			name.append("FMOD -> " + std::to_string(eraseCount) + " channels were deleted");
			POWLOGNORMAL(name);
		}

	}
}

powe::FMOD2DSound::FMOD2DSound(uint32_t nbChannels)
	: m_FmodSoundImpl(std::make_unique<FMODSoundImpl>(nbChannels))
{
}

powe::FMOD2DSound::~FMOD2DSound() = default;

powe::SoundID powe::FMOD2DSound::RegisterSoundEntity(const std::string& filePath, bool shouldPreloaded)
{
	return m_FmodSoundImpl->RegisterSoundEntity(filePath, shouldPreloaded);
}

void powe::FMOD2DSound::UnRegisterSoundEntity(SoundID id)
{
	m_FmodSoundImpl->UnRegisterSoundEntity(id);
}

bool powe::FMOD2DSound::IsPlaying(SoundID id) const
{
	return m_FmodSoundImpl->IsPlaying(id);
}

powe::ChannelID powe::FMOD2DSound::Play(SoundID id, const SoundInfo& soundInfo)
{
	return m_FmodSoundImpl->Play(id, soundInfo);
}

powe::ChannelID powe::FMOD2DSound::PlayImmediate(const std::string& filePath, const SoundInfo& soundInfo)
{
	return m_FmodSoundImpl->PlayImmediate(filePath, soundInfo);
}

void powe::FMOD2DSound::Update()
{
	//m_FmodSoundImpl->Update();
}

void powe::FMOD2DSound::Stop(SoundID id)
{
	m_FmodSoundImpl->Stop(id);
}

