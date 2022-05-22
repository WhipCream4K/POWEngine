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
        void UnRegisterSoundEntity(SoundID id) override;
        bool IsPlaying(ChannelID id) const override;
        ChannelID Play(SoundID id, const SoundInfo& soundInfo) override;
        ChannelID PlayImmediate(const std::string& filePath, const SoundInfo& soundInfo) override;
        void Update() override;
        void Stop(ChannelID id) override;

    private:

        class FMODSoundImpl;
        OwnedPtr<FMODSoundImpl> m_FmodSoundImpl;
    };
}

 
