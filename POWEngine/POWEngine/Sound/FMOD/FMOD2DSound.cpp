#include "pch.h"
#include "FMOD2DSound.h"

#include <fmod.hpp>
#include <fmod_errors.h>

#include <future>
#include <queue>
#include "POWEngine/LockFree/LFQueue.h"
#include "AudioClip.h"
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
	}

	~FMODSoundImpl();

	SoundID RegisterSoundEntity(const std::string& filePath);
	void UnRegisterSoundEntity(SoundID id);
	void InitializeThread();
	ChannelID Play(SoundID id, const SoundInfo& info);
	ChannelID PlayImmediate(const std::string& filePath, const SoundInfo& soundInfo);
	void Stop(ChannelID id);
	bool IsPlaying(ChannelID id);
	void Update();


private:

	void RunMainPlaySound();

	FMOD::System* m_FMODSystem{};
	uint32_t m_MaxChannelsCnt{};

	// Audio
	std::mutex m_AudioMapMutex{};
	std::unordered_map<SoundID, SharedPtr<AudioClip>> m_AudioClip{};
	std::unordered_set<std::string> m_PathCheck{};
	std::atomic<SoundID> m_AudioClipCnt{};

	// Channels
	std::mutex m_ChannelMutex{};
	std::vector<SharedPtr<Channel>> m_ActiveChannels{};
	std::atomic<ChannelID> m_ChannelCnt{};

	std::future<void> m_SoundThread{};
	std::condition_variable m_MainSoundThreadCV{};
	bool m_IsThreadExit{}; 

	struct SoundQueue
	{
		SoundQueue(const SharedPtr<AudioClip>& clip,Channel* channel)
			: aClip(clip)
			, aChannel(channel)
		{
		}

		SoundQueue() = default;

		SharedPtr<AudioClip> aClip{};
		Channel* aChannel{};
	};

	std::mutex m_PlayQueueMutex{};
	LFQueue<SoundQueue> m_PlayQueue;
};

powe::FMOD2DSound::FMODSoundImpl::~FMODSoundImpl()
{
	m_FMODSystem->release();
	m_FMODSystem = nullptr;
	m_IsThreadExit = true;
}

powe::SoundID powe::FMOD2DSound::FMODSoundImpl::RegisterSoundEntity(const std::string& )
{
	//const auto& [listItr, emplaceResult] { m_PreloadedSoundHandle.try_emplace(filePath, m_PreLoadedSoundCnt) };
	//if (emplaceResult)
	//{
	//	FMOD::Sound* soundInst{};
	//	const auto result = m_FMODSystem->createSound(filePath.c_str(), FMOD_2D | FMOD_CREATESAMPLE | FMOD_LOOP_OFF,
	//		nullptr, &soundInst);

	//	if (result != FMOD_OK)
	//		throw std::runtime_error(FMOD_ErrorString(result));

	//	SoundMemo memo{};
	//	memo.filePath = filePath;
	//	memo.soundInst = soundInst;

	//	m_PreLoadedSounds[m_PreLoadedSoundCnt] = memo;
	//	++m_PreLoadedSoundCnt;
	//}

	//return listItr->second;
	return {};
}

void powe::FMOD2DSound::FMODSoundImpl::UnRegisterSoundEntity(SoundID )
{
	
}

void powe::FMOD2DSound::FMODSoundImpl::InitializeThread()
{
	m_SoundThread = std::async(std::launch::async, &FMODSoundImpl::RunMainPlaySound, this);
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

	const SharedPtr<Channel> channel{ std::make_shared<Channel>(m_ChannelCnt++,info) };

	{
		std::scoped_lock lock{ m_ChannelMutex };
		m_ActiveChannels.emplace_back(channel);
	}

	const SoundQueue soundQueue{clip,channel.get()};
	m_PlayQueue.Push(soundQueue);

	m_MainSoundThreadCV.notify_one();

	return channel->GetID();
}

powe::ChannelID powe::FMOD2DSound::FMODSoundImpl::PlayImmediate(const std::string& filePath, const SoundInfo& soundInfo)
{
	const SharedPtr<AudioClip> audioClip{ std::make_shared<AudioClip>(0,filePath)};

	const SharedPtr<Channel> channel{ std::make_shared<Channel>(m_ChannelCnt++,soundInfo) };

	{
		std::scoped_lock lock{ m_ChannelMutex };
		m_ActiveChannels.emplace_back(channel);
	}

	const SoundQueue soundQueue{audioClip,channel.get()};
	m_PlayQueue.Push(soundQueue);

	m_MainSoundThreadCV.notify_one();

	return channel->GetID();
}

void powe::FMOD2DSound::FMODSoundImpl::Stop(ChannelID id)
{
	std::ranges::borrowed_iterator_t<const std::vector<SharedPtr<Channel>>&> findItr{};
	{
		std::scoped_lock lock{ m_ChannelMutex };
		
		findItr = std::ranges::find_if(m_ActiveChannels, [&id](const SharedPtr<Channel>& channel)
			{
				return channel->GetID() == id;
			});

		if (findItr == m_ActiveChannels.end())
			return;
	}

	findItr->get()->Stop();
}

bool powe::FMOD2DSound::FMODSoundImpl::IsPlaying(ChannelID id)
{
	std::ranges::borrowed_iterator_t<const std::vector<SharedPtr<Channel>>&> findItr{};

	{
		std::scoped_lock lock{ m_ChannelMutex };

		findItr = std::ranges::find_if(m_ActiveChannels, [&id](const SharedPtr<Channel>& channel)
			{
				return channel->GetID() == id;
			});

		if (findItr == m_ActiveChannels.end())
			return false;
	}

	return findItr->get()->IsPlaying();
}

void powe::FMOD2DSound::FMODSoundImpl::Update()
{
	{
		std::scoped_lock lock{ m_ChannelMutex };
		m_ActiveChannels.erase(std::ranges::find_if(m_ActiveChannels, [](const SharedPtr<Channel>& channel)
			{
				return !channel->IsPlaying();
			}),m_ActiveChannels.end());
	}
}


void powe::FMOD2DSound::FMODSoundImpl::RunMainPlaySound()
{
	// this is single a producer so we don't need atomic bool check
	while (!m_IsThreadExit)
	{
		{
			std::unique_lock lock{ m_PlayQueueMutex };

			m_MainSoundThreadCV.wait(lock, [this]()
				{
					return !m_PlayQueue.Empty();
				});
		}

		const SoundQueue element{m_PlayQueue.Front()};
		m_PlayQueue.Pop();

		if (!element.aClip->IsLoaded())
			element.aClip->LoadStream(m_FMODSystem);

		element.aChannel->Play(m_FMODSystem,element.aClip->GetSound());
	}
}

powe::FMOD2DSound::FMOD2DSound(uint32_t nbChannels)
	: m_FmodSoundImpl(std::make_unique<FMODSoundImpl>(nbChannels))
{
}

powe::FMOD2DSound::~FMOD2DSound() = default;

powe::SoundID powe::FMOD2DSound::RegisterSoundEntity(const std::string& , bool )
{
	return {};
}

void powe::FMOD2DSound::UnRegisterSoundEntity(SoundID )
{

}

//powe::SoundID powe::FMOD2DSound::PreLoadSoundToMemory(const std::string & filePath)
//{
//	return m_FmodSoundImpl->PreLoadSoundToMemory(filePath);
//}
//
//void powe::FMOD2DSound::RemovePreLoadSound(SoundID id)
//{
//	m_FmodSoundImpl->RemovePreLoadSound(id);
//}


bool powe::FMOD2DSound::IsPlaying(ChannelID id) const
{
	return m_FmodSoundImpl->IsPlaying(id);
}

powe::ChannelID powe::FMOD2DSound::Play(SoundID id, const SoundInfo & soundInfo)
{
	return m_FmodSoundImpl->Play(id, soundInfo);
}

powe::ChannelID powe::FMOD2DSound::PlayImmediate(const std::string& filePath, const SoundInfo& soundInfo)
{
	return m_FmodSoundImpl->PlayImmediate(filePath, soundInfo);
}

void powe::FMOD2DSound::Update()
{
	m_FmodSoundImpl->Update();
}

void powe::FMOD2DSound::Stop(ChannelID id)
{
	m_FmodSoundImpl->Stop(id);
}

