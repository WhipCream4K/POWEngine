#pragma once

#include <string>
#include "Sound/SoundInfo.h"

namespace FMOD
{
	class Sound;
	class System;
}

namespace powe
{
	class FMODAudioClip
	{
	public:

		explicit FMODAudioClip(SoundID id,const std::string& filePath);

		bool IsLoaded() const;
		void LoadStream(FMOD::System* system);
		void LoadCompressed(FMOD::System* system);

		const std::string& GetFilePath() const { return m_FilePath; }
		FMOD::Sound* GetSound() const { return m_SoundInst; }

		~FMODAudioClip();

	private:

		FMOD::Sound* m_SoundInst{};
		SoundID m_Id{};
		std::string m_FilePath{};
	};
}


