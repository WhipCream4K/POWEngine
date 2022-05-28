#pragma once

#include "ListsOfKeys.h"
#include <variant>

namespace powe
{
	//struct InputState
	//{
	//	InputEvent keyEvent{ InputEvent::IE_None };
	//	uint8_t userIndex{};
	//	float axisValue{};
	//};

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

	struct MouseData
	{
		MouseCharKey keyCode{};
		std::variant<MouseWheelDelta, MousePos> axisData{};
	};

	struct GamepadAxisData
	{
		float LThumbX{}, LThumbY{};
		float RThumbX{}, RThumbY{};
		float LShoulder{}, RShoulder{};
		uint8_t playerIndex{};
	};

	struct GamepadButtonData
	{
		uint16_t buttons{};
		uint8_t playerIndex{};
	};

	// The implementation is almost like union
	using HardWareInputData = std::variant<MouseData, KeyboardData,GamepadButtonData,GamepadAxisData>;

}


