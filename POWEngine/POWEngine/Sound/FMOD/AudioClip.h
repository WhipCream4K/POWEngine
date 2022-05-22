#pragma once

#include "POWEngine/Sound/SoundTypes.h"
#include "POWEngine/Sound/SoundInfo.h"

namespace FMOD
{
	class Sound;
	class System;
}

namespace powe
{
	class AudioClip
	{
	public:

		explicit AudioClip(SoundID id,const std::string& filePath);

		bool IsLoaded() const;
		void LoadStream(FMOD::System* system);
		FMOD::Sound* GetSound() const { return m_SoundInst; }

		~AudioClip();

	private:

		FMOD::Sound* m_SoundInst{};
		SoundID m_Id{};
		std::string m_FilePath{};
	};
}


