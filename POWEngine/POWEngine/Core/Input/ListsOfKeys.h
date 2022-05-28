#pragma once

#include <cstdint>

enum class InputEvent
{
	IE_None,
	IE_Released = 0b10,
	IE_Pressed = 0b01,
	IE_Down = 0b11,
};

class MouseKey
{
public:
	enum : uint8_t
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
};



class KeyboardSysKey
{
public:
	enum : uint8_t
	{
		KS_Ctrl,
		KS_Shift,
		KS_Alt,
		KS_System,
	};
};



class GamepadKey
{
	enum : uint16_t
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
};

class Keyboard
{
public:

	enum  : uint8_t
	{
		A = 0,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,
		Num0,
		Num1,
		Num2,
		Num3,
		Num4,
		Num5,
		Num6,
		Num7,
		Num8,
		Num9,
		Escape,
		LControl,
		LShift,
		LAlt,
		LSystem,
		RControl,
		RShift,
		RAlt,
		RSystem,
		Menu,
		LBracket,
		RBracket,
		Semicolon,
		Comma,
		Period,
		Quote,
		Slash,
		Backslash,
		Tilde,
		Equal,
		Hyphen,
		Space,
		Enter,
		Backspace,
		Tab,
		PageUp,
		PageDown,
		End,
		Home,
		Insert,
		Delete,
		Add,
		Subtract,
		Multiply,
		Divide,
		Left,
		Right,
		Up,
		Down,
		Numpad0,
		Numpad1,
		Numpad2,
		Numpad3,
		Numpad4,
		Numpad5,
		Numpad6,
		Numpad7,
		Numpad8,
		Numpad9,
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		F13,
		F14,
		F15,
		Pause,

		KeyCount
	};
};





//static constexpr uint32_t MaxHardWareInputs{UINT8_MAX + }

//namespace powe
//{
//	enum class InputDevice;
//
//	//inline static bool IsKeyboardEvent(uint8_t eventId)
//	//{
//	//	if (eventId == EventType::KeyPressed ||
//	//		eventId == EventType::KeyReleased)
//	//		return true;
//
//	//	return false;
//	//}
//
//	//inline static bool IsMouseEvent(uint8_t eventId)
//	//{
//	//	if(eventId >= EventType::MouseWheelScrolled &&
//	//		eventId < EventType::MouseEntered)
//	//	{
//	//		return true;
//	//	}
//
//	//	return false;
//	//}
//
//	struct InputState
//	{
//		InputEvent keyEvent{ InputEvent::IE_None };
//		uint8_t userIndex{};
//		float axisValue{};
//	};
//
//	struct WndMessageHWIdx
//	{
//		uint8_t idx;
//		InputDevice hardWare;
//	};
//	
//}
