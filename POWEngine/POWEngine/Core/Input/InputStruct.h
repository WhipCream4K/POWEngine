#pragma once

#include "InputTypes.h"
#include <variant>

namespace powe
{
	struct InputState
	{
		InputEvent keyEvent{ InputEvent::IE_None };
		uint8_t userIndex{};
		float axisValue{};
	};

	// reduce 8 bytes of information when separate wheel data out 
	struct MousePos
	{
		int relativePosX{};
		int relativePosY{};
	};

	using MouseWheelDelta = float;

	struct KeyboardData
	{
		uint8_t keyCode{};
		uint8_t sysKey{};
	};

	using MouseCharKey = uint8_t;

	using MouseData = std::variant<MouseCharKey, MouseWheelDelta, MousePos>;

	// The implementation is almost like union
	using HardWareInputData = std::variant<MouseData, KeyboardData>;
}