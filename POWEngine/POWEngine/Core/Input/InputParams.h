#pragma once

#include <cstdint>

enum class InputEvent
{
	IE_Pressed,
	IE_Released,
	IE_Down,
	IE_None
};

enum class MouseKey : uint8_t
{
	MK_Left,
	MK_Right,
	MK_Middle,
	MK_Thumb1,
	MK_Thumb2,
	MK_AxisX,
	MK_AxisY,
	MK_None
};

enum class GamePadKey : uint16_t
{
	GPK_None = 0,
	GPK_DPAD_Up = (1 << 0),
	GPK_DPAD_Down = (1 << 1),
	GPK_DPAD_Left = (1 << 2),
	GPK_DPAD_Right = (1 << 3),
	GPK_Start = (1 << 4),
	GPK_Back = (1 << 5),
	GPK_Left_Thumb = (1 << 6),
	GPK_Right_Thumb = (1 << 7),
	GPK_Left_Shoulder = (1 << 8),
	GPK_Right_Shoulder = (1 << 9),
	GPK_Right_AxisX = (1 << 10),
	GPK_Right_AxisY,
	GPK_Left_AxisX,
	GPK_Left_AxisY,
	GPK_A = (1 << 12),
	GPK_B = (1 << 13),
	GPK_X = (1 << 14),
	GPK_Y = (1 << 15),


	GPK_KEY_CHECK = 0b111100111111111
};

// NOTE: Change this if new input system is implemented
enum EventType : uint8_t
{
	Closed,                 //!< The window requested to be closed (no data)
	Resized,                //!< The window was resized (data in event.size)
	LostFocus,              //!< The window lost the focus (no data)
	GainedFocus,            //!< The window gained the focus (no data)
	TextEntered,            //!< A character was entered (data in event.text)
	KeyPressed,             //!< A key was pressed (data in event.key)
	KeyReleased,            //!< A key was released (data in event.key)
	MouseWheelScrolled,     //!< The mouse wheel was scrolled (data in event.mouseWheelScroll)
	MouseButtonPressed,     //!< A mouse button was pressed (data in event.mouseButton)
	MouseButtonReleased,    //!< A mouse button was released (data in event.mouseButton)
	MouseMoved,             //!< The mouse cursor moved (data in event.mouseMove)
	MouseEntered,           //!< The mouse cursor entered the area of the window (no data)
	MouseLeft,              //!< The mouse cursor left the area of the window (no data)
	JoystickButtonPressed,  //!< A joystick button was pressed (data in event.joystickButton)
	JoystickButtonReleased, //!< A joystick button was released (data in event.joystickButton)
	JoystickMoved,          //!< The joystick moved along an axis (data in event.joystickMove)
	JoystickConnected,      //!< A joystick was connected (data in event.joystickConnect)
	JoystickDisconnected,   //!< A joystick was disconnected (data in event.joystickConnect)
	TouchBegan,             //!< A touch event began (data in event.touch)
	TouchMoved,             //!< A touch moved (data in event.touch)
	TouchEnded,             //!< A touch event ended (data in event.touch)
	SensorChanged,          //!< A sensor value changed (data in event.sensor)

	Count                   //!< Keep last -- the total number of event types
};

namespace powe
{
	enum class InputDevice;

	inline static bool IsKeyboardEvent(uint8_t eventId)
	{
		if (eventId == EventType::KeyPressed ||
			eventId == EventType::KeyReleased)
			return true;

		return false;
	}

	inline static bool IsMouseEvent(uint8_t eventId)
	{
		if(eventId >= EventType::MouseWheelScrolled &&
			eventId < EventType::MouseEntered)
		{
			return true;
		}

		return false;
	}

	struct InputState
	{
		InputEvent keyEvent{ InputEvent::IE_None };
		uint8_t userIndex{};
		float axisValue{};
	};

	struct WndMessageHWIdx
	{
		uint8_t idx;
		InputDevice hardWare;
	};
	
}
