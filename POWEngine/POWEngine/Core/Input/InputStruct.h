#pragma once

#include "ListsOfKeys.h"
#include <variant>

namespace powe
{
	struct InputState
	{
		InputEvent keyEvent{ InputEvent::IE_None };
		uint8_t userIndex{};
		float axisValue{};
	};

	struct KeyState
	{
		float axisThisFrame{};
		bool inputLastFrame{};
		bool inputThisFrame{};
	};

	// reduce 8 bytes of information when separate wheel data out 
	// 24 bytes
	struct MousePos
	{
		float deltaPosX{};
		float deltaPosY{};
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

	//using MouseData = std::variant<MouseCharKey, MouseWheelDelta, MousePos>;

	struct MouseData
	{
		MouseCharKey keyCode{};
		std::variant<MouseWheelDelta, MousePos> axisData{};
	};

	// The implementation is almost like union
	using HardWareInputData = std::variant<MouseData, KeyboardData>;

	// Just some helper function to get mouse data
	template<typename T>
	inline static decltype(auto) GetMouseData(const HardWareInputData& hData)
	{
		return std::get<T>(std::get<MouseData>(hData));
	}
}


