#pragma once

namespace powe
{
	using SoundID = uint32_t;
	using ChannelID = int;

	struct SoundInfo
	{
		float volume{1.0f};
		float pitch{1.0f};
		bool isLooped{};
	};
}
