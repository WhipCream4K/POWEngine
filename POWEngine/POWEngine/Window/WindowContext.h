#pragma once

#include <type_traits>
#include <array>
#include <any>
#include <string>
#include <vector>
#include "POWEngine/Core/Input/InputStruct.h"

namespace powe
{
	constexpr uint32_t MinimumWindowEventCnt{ 15 };

	struct MessageBus
	{
		std::any data{};
		uint8_t eventId{};
		//InputDevice inDevice;
	};

	enum class InputDevice;

	struct HardwareBus
	{
		HardWareInputData hData{};
		InputDevice inDevice{};
		uint8_t eventId{};
	};

	struct WindowMessages
	{
		std::array<MessageBus, MinimumWindowEventCnt> wndMessages{};
		int totalMessages{};
	};

	struct HardwareMessages
	{
		std::array<HardwareBus, MinimumWindowEventCnt> hwMessages{};
		int totalMessages{};
	};

	using OtherWindowParams = std::vector<std::byte>;
}

