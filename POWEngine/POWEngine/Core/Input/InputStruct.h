#pragma once

#include "POWEngine/Window/WindowContext.h"
#include "POWEngine/Math/Math.h"
#include "ListsOfKeys.h"
#include "Key.h"
#include <variant>

namespace powe
{
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

	// Keyboard Data per key
	struct KeyboardData
	{
		uint8_t keyCode{};
		uint8_t sysKey{};
		bool isPressed{};
	};

	using MouseKeyType = uint8_t;

	struct MouseKeyData
	{
		uint8_t keyCode{};
		bool isPressed{};
	};

	struct GamepadData
	{
		float thumbAxisData[GamepadKey::ThumbAxisCount]{};
		float LShoulder{}, RShoulder{};
		uint16_t buttons{};
		uint8_t playerIndex{};
	};
	
	// struct MouseKeyData
	// {
	// 	glm::uvec2 mousePosTopLeft{};
	// };

	// The implementation is almost like union
	// using HardWareInputData = std::variant<MouseKeyData, MouseWheelDelta, MousePos, KeyboardData, GamepadData>;

	// Button data is data that has press and release data
	using ButtonData = std::variant<MouseKeyData,KeyboardData, GamepadData>;

	enum class EventType
	{
		MouseButton,
		MouseWheelMoved,
		MouseMoved,
		KeyboardButton,
		Gamepad
	};

	struct HardwareData
	{
		// Data from window polling
		ButtonData hData{};
		InputDevice inDevice{};
		uint8_t eventId{};
	};

	struct MouseAxisData
	{
		glm::uvec2 windowDimension{};
		glm::ivec2 mousePos{}; // mouse position in the polling window relative to the top left corner
		float deltaWheel{};
	};

	struct HardwareMessages
	{
		std::array<HardwareData, MinimumWindowEventCnt> hwMessages{};
		MouseAxisData mouseAxis{};
		int totalMessages{};
	};

}


