#pragma once

#include <cstdint>

enum class InputEvent
{
	IE_None,
	IE_Released = 0b01,
	IE_Pressed = 0b10,
	IE_Down = 0b11,
};

class MouseKey
{
public:

	enum : uint16_t
	{
		MK_Left,
		MK_Right,
		MK_Middle,
		MK_Thumb1,
		MK_Thumb2,

		MK_AxisX,
		MK_AxisY,

		MK_MiddleUp,
		MK_MiddleDown,
	};

	static constexpr uint8_t AxisCount{ 1 };
	static constexpr uint8_t KeyCount{5};
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
public:

	enum : uint8_t
	{
		GPK_DPAD_Up = 0,
		GPK_DPAD_Down = 1,
		GPK_DPAD_Left = 2,
		GPK_DPAD_Right = 3,
		GPK_Start = 4,
		GPK_Back = 5,
		GPK_Left_Thumb = 6,
		GPK_Right_Thumb = 7,
		GPK_Left_Shoulder = 8,
		GPK_Right_Shoulder = 9,

		GPK_A,
		GPK_B,
		GPK_X,
		GPK_Y,

		GPK_Right_AxisX,
		GPK_Right_AxisY,
		GPK_Left_AxisX,
		GPK_Left_AxisY,

		GPK_Count
	};

	static uint16_t GetKeyCodeFromBitPos(int bitPos)
	{
		if (bitPos > 9)
			return uint16_t(1 << (bitPos + 2));

		return uint16_t(1 << bitPos);
	}

	static int GetThumbIndexDataFromKeyCode(int keycode)
	{
		return keycode - int(GPK_Right_AxisX);
	}

	static constexpr int AxisCount{ 6 };
	static constexpr int ThumbAxisCount{ 4 };
	static constexpr int ButtonCount{ 14 };
};

class Keyboard
{
public:

	enum : uint8_t
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



