#include "pch.h"
#include "FMOD2DSound.h"

#include <fmod.hpp>
#include <fmod_errors.h>

#include <future>
#include <queue>
#include "POWEngine/LockFree/LFQueue.h"

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

	SoundID PreLoadSoundToMemory(const std::string& filePath);
	void RemovePreLoadSound(SoundID id);
	void InitializeThread();
	ChannelID Play(SoundID id, const SoundInfo& info);
	bool IsPlaying(ChannelID id) const;
	

private:

	void RunMainPlaySound();
	void RunReservedPlaySound();

	FMOD::System* m_FMODSystem{};
	uint32_t m_MaxChannelsCnt{};

	struct SoundMemo
	{
		std::string filePath{};
		FMOD::Sound* soundInst{};
	};

	// Loaded Sounds in memory
	std::mutex m_PreLoadSoundMutex;
	std::unordered_map<std::string, SoundID> m_PreloadedSoundHandle;
	std::unordered_map<SoundID, SoundMemo> m_PreLoadedSounds;
	SoundID m_PreLoadedSoundCnt{};

	// Channels
	struct Channel
	{
		FMOD::Channel* channelInst{};
		FMOD::Sound* soundInst{};
		std::string filePath{};
		SoundID soundID{};
		SoundInfo soundInfo{};
	};

	ChannelID m_CurrentChannelsID{};
	std::vector<Channel> m_Channels;
	std::vector<Channel> m_WaitingChannels{};


	std::future<void> m_SoundThread{};
	std::condition_variable m_MainSoundThreadCV{};
	bool m_IsThreadExit{};
	std::mutex m_SoundQueueMutex{};
	std::queue<ChannelID> m_ActiveChannels;


	std::future<void> m_ReservedSoundThread{};
	std::condition_variable m_ReservedSoundThreadCV{};
	std::mutex m_ReservedThreadMutex{};
	//std::vector<std::vector<Channel>> m_ToBePlayedSound{};
	std::unordered_map<ChannelID, Channel> m_ToBePlayedSound{};
};

powe::FMOD2DSound::FMODSoundImpl::~FMODSoundImpl()
{
	m_FMODSystem->release();
	m_FMODSystem = nullptr;
	m_IsThreadExit = true;
}

powe::SoundID powe::FMOD2DSound::FMODSoundImpl::PreLoadSoundToMemory(const std::string& filePath)
{
	const auto& [listItr, emplaceResult] { m_PreloadedSoundHandle.try_emplace(filePath, m_PreLoadedSoundCnt) };
	if (emplaceResult)
	{
		FMOD::Sound* soundInst{};
		const auto result = m_FMODSystem->createSound(filePath.c_str(), FMOD_2D | FMOD_CREATESAMPLE | FMOD_LOOP_OFF,
			nullptr, &soundInst);

		if (result != FMOD_OK)
			throw std::runtime_error(FMOD_ErrorString(result));

		SoundMemo memo{};
		memo.filePath = filePath;
		memo.soundInst = soundInst;

		m_PreLoadedSounds[m_PreLoadedSoundCnt] = memo;
		++m_PreLoadedSoundCnt;
	}

	return listItr->second;
}

void powe::FMOD2DSound::FMODSoundImpl::RemovePreLoadSound(SoundID id)
{
	if (m_PreLoadedSounds.contains(id))
	{
		const auto& soundMemo{ m_PreLoadedSounds[id] };

		soundMemo.soundInst->release();

		m_PreloadedSoundHandle.erase(soundMemo.filePath);
		m_PreLoadedSounds.erase(id);
	}
}

void powe::FMOD2DSound::FMODSoundImpl::InitializeThread()
{
	m_SoundThread = std::async(std::launch::async, &FMODSoundImpl::RunMainPlaySound, this);
}

powe::ChannelID powe::FMOD2DSound::FMODSoundImpl::Play(SoundID id, const SoundInfo& info)
{
	const ChannelID channelID{ m_CurrentChannelsID++ };

	Channel channel{};
	channel.soundInfo = info;

	{
		std::scoped_lock lock{ m_PreLoadSoundMutex };
		const auto soundItr{ m_PreLoadedSounds.find(id) };
		if (soundItr != m_PreLoadedSounds.end())
		{
			channel.soundInst = soundItr->second.soundInst;
		}
	}


	{
		std::scoped_lock lock{m_SoundQueueMutex};
		bool isPlaying{};
		m_Channels[int(channelID % m_MaxChannelsCnt)].channelInst->isPlaying(&isPlaying);
		if (!isPlaying)
			m_ActiveChannels.push(channelID);

		//m_Channels[int(channelId % m_MaxChannelsCnt)] = channel;
		//m_ActiveChannels.push(channelId);
	}

	m_MainSoundThreadCV.notify_one();

	return channelID;
}

bool powe::FMOD2DSound::FMODSoundImpl::IsPlaying(ChannelID id) const
{
	Channel tempChannel{};

	{
		std::scoped_lock lock{ m_SoundQueueMutex };
		tempChannel = m_Channels[id / m_MaxChannelsCnt];
	}

	bool isPlaying{};
	tempChannel.channelInst->isPlaying(&isPlaying);
	return isPlaying;
}


void powe::FMOD2DSound::FMODSoundImpl::RunMainPlaySound()
{
	// this is single a producer so we don't need atomic bool check
	while (!m_IsThreadExit)
	{
		Channel tempChannel{};

		{
			std::unique_lock lock{ m_SoundQueueMutex };

			m_MainSoundThreadCV.wait(lock, [this]()
				{
					return !m_ActiveChannels.empty();
				});

			tempChannel = m_Channels[m_ActiveChannels.front()];
			m_ActiveChannels.pop();
		}

		FMOD::Sound* toPlaySound{};
		if(!tempChannel.soundInst)
		{
			auto result = m_FMODSystem->createStream(tempChannel.filePath.c_str(),
				FMOD_2D | FMOD_CREATESTREAM | FMOD_LOOP_OFF,
				nullptr, &toPlaySound);

			if (result != FMOD_OK)
				throw std::runtime_error(FMOD_ErrorString(result));
		}
		else
		{
			toPlaySound = tempChannel.soundInst;
		}

		//{
		//	std::unique_lock lock{ m_ThreadMutex };

		//	m_TaskWait.wait(lock, [this]()
		//		{
		//			return !m_SoundQueue.Empty();
		//		});
		//}

		//const FMODSoundInfo soundInst{  };
		//m_SoundQueue.Pop();

		FMOD::Channel* channel{};
		const auto result = m_FMODSystem->playSound(toPlaySound, nullptr, true, &channel);

		//if (result == FMOD_OK)
		//{
		//	const int loopCount{ soundInst.isLooped ? -1 : 0 };
		//	channel->setLoopCount(loopCount);
		//	channel->setVolume(soundInst.volume);
		//	channel->setPitch(soundInst.pitch);
		//	channel->setPaused(false);
		//}
		
		{
			std::scoped_lock lock{ m_SoundQueueMutex };
			m_Channels[2].channelInst = channel;

			//for (const auto& channel : m_Channels)
			//{
			//	bool isPlaying{};
			//	channel.channelInst->isPlaying(&isPlaying);
			//	if (!isPlaying)
			//	{
			//		m_ActiveChannels.push(0);
			//	}
			//}
		}


	}
}

void powe::FMOD2DSound::FMODSoundImpl::RunReservedPlaySound()
{

	{
		std::unique_lock lock{ m_ReservedThreadMutex };

		m_ReservedSoundThreadCV.wait(lock, [this]()
			{
				return !m_ToBePlayedSound.empty();
			});

		for (const auto& [channelID,soundData] : m_ToBePlayedSound)
		{
			{
				std::scoped_lock mainSoundThreadLock{ m_SoundQueueMutex };
				FMOD::Channel* tempChannel{ m_Channels[channelID].channelInst };
				if(tempChannel)
				{
					bool isPlaying{};
					tempChannel->isPlaying(&isPlaying);
					if(!isPlaying)
					{
						m_ActiveChannels.push(channelID);
						m_Channels[channelID] = soundData;
					}
				}
			}
		}
	}
	
}

powe::FMOD2DSound::FMOD2DSound(uint32_t nbChannels)
	: m_FmodSoundImpl(std::make_unique<FMODSoundImpl>(nbChannels))
{
}

powe::FMOD2DSound::~FMOD2DSound() = default;

powe::SoundID powe::FMOD2DSound::RegisterSoundEntity(const std::string& filePath, bool shouldLoaded)
{
	
}

powe::SoundID powe::FMOD2DSound::PreLoadSoundToMemory(const std::string & filePath)
{
	return m_FmodSoundImpl->PreLoadSoundToMemory(filePath);
}

void powe::FMOD2DSound::RemovePreLoadSound(SoundID id)
{
	m_FmodSoundImpl->RemovePreLoadSound(id);
}


bool powe::FMOD2DSound::IsPlaying(SoundID id)
{


	return true;
}

powe::ChannelID powe::FMOD2DSound::Play(SoundID id, const SoundInfo & soundInfo)
{

}


void powe::FMOD2DSound::Stop(SoundID id)
{

}
