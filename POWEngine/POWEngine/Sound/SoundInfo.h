#pragma once

namespace powe
{
	struct SoundInfo
	{
		SoundInfo() = default;

		SoundInfo(const SoundInfo&) = default;
		SoundInfo& operator=(const SoundInfo&) = default;
		SoundInfo(SoundInfo&&) noexcept = default;
		SoundInfo& operator=(SoundInfo&&) noexcept = default;
		~SoundInfo() = default;

		float volume{1.0f};
		float pitch{1.0f};
		bool isLooped{};
	};
}
