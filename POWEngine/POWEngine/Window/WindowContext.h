#pragma once

#include <type_traits>
#include <array>
#include <any>
#include <string>
#include <vector>

namespace powe
{
	constexpr uint32_t MinimumWindowEventCnt{ 15 };

	struct MessageBus
	{
		std::any data{};
		int size = 0;
		uint8_t eventId = 0;
	};

	struct WindowMessages
	{
		std::array<MessageBus, MinimumWindowEventCnt> wndMessages{};
		int totalMessages{};
	};

	using OtherWindowParams = std::vector<std::byte>;
}

