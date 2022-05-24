#pragma once

#include "POWEngine/Core/Components/BaseComponent.h"
#include "POWEngine/Sound/SoundTypes.h"
#include "POWEngine/Sound/SoundInfo.h"

namespace powe
{
	class AudioComponent : public Component<AudioComponent>
	{
	public:

		AudioComponent() = default;
		AudioComponent(const std::string& filePath,bool shouldPreloaded = false);
		AudioComponent(const std::string& filePath,const SoundInfo& info, bool shouldPreloaded = false);

		AudioComponent(const AudioComponent&) = default;
		AudioComponent& operator=(const AudioComponent&) = default;
		AudioComponent(AudioComponent&&) = default;
		AudioComponent& operator=(AudioComponent&&) = default;
		~AudioComponent() override;


		void Play();
		void Play(const SoundInfo& info);
		bool IsPlaying() const;
		void Stop() const;

	private:

		SoundInfo m_Info{};
		SoundID m_SoundID{};
		OwnedPtr<int> m_hey{};
		ChannelID m_UsedChannel{};
	};
}


