#pragma once
#include "POWEngine/Sound/SoundSystem.h"

namespace powe
{
	class FMOD2DSound : public SoundSystem
    {
    public:

        FMOD2DSound(uint32_t nbChannels);
        FMOD2DSound(const FMOD2DSound&) = delete;
        FMOD2DSound& operator=(const FMOD2DSound&) = delete;
        FMOD2DSound(FMOD2DSound&&) = delete;
        FMOD2DSound& operator=(FMOD2DSound&&) = delete;
        ~FMOD2DSound() override;

    public:

        SoundID RegisterSoundEntity(const std::string& filePath, bool shouldLoaded) override;
        SoundID PreLoadSoundToMemory(const std::string& filePath) override;
        void RemovePreLoadSound(SoundID id) override;
        bool IsPlaying(ChannelID id) override;
        ChannelID Play(SoundID id, const SoundInfo& soundInfo) override;
        //void Play(SoundID id, SoundInfo soundInfo) override;
        //void Play(SoundID id, WeakPtr<SoundInfo> soundInfo) override;

        void Stop(ChannelID id) override;

    private:

        class FMODSoundImpl;
        OwnedPtr<FMODSoundImpl> m_FmodSoundImpl;
    };
}

 
