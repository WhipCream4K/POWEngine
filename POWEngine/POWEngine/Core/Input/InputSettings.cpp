#include "pch.h"
#include "InputSettings.h"
#include "POWEngine/Logger/LoggerUtils.h"
#include "POWEngine/Window/WindowEvents.h"

#if USE_SFML_WINDOW

#include <SFML/Window.hpp>

//void powe::InputSettings::ParseWndMessages(
//	const WindowMessages& winMessages,
//	const std::array<WndMessageHWIdx, MinimumWindowEventCnt>& inputHardWareIdx)
//{
//
//	Key itrKey{};
//
//	// KeyPool lookUp vs iterate KeyPool
//	for (const auto& input : inputHardWareIdx)
//	{
//		// TODO: Maybe take care of out of range
//
//		// maybe overflow operation uint8_t -> int
//		const MessageBus msgBus{ winMessages.wndMessages[int(input.idx)] };
//
//		switch (input.hardWare)
//		{
//		case InputDevice::D_Keyboard:
//		{
//			itrKey.inputDevice = InputDevice::D_Keyboard;
//
//			// TODO: Take care of syskey
//			const auto& sfKey{ std::any_cast<sf::Event::KeyEvent>(msgBus.data) };
//			itrKey.keyCode = uint16_t(sfKey.code);
//
//			ParseKeyboardKey(msgBus.eventId, itrKey);
//
//			break;
//		}
//		case InputDevice::D_Mouse:
//		{
//			try
//			{
//				itrKey.inputDevice = InputDevice::D_Mouse;
//
//				const auto& sfMouse{ std::any_cast<sf::Event::MouseButtonEvent>(msgBus.data) };
//				itrKey.keyCode = uint16_t(sfMouse.button);
//
//				ParseMouseKey(msgBus.eventId, itrKey);
//			}
//			catch (const std::exception& some)
//			{
//				//POWLOGERROR(some.what());
//				POWLOGINFO(some.what());
//			}
//
//			break;
//		}
//		case InputDevice::D_Gamepad:
//
//			//TODO: Support the gamepad
//
//
//			break;
//		default:;
//		}
//	}
//
//	//for (const auto& keyPool : m_MainKeyPool)
//	//{
//	//	switch (keyPool.first.inputDevice)
//	//	{
//	//	case InputDevice::D_Keyboard:
//	//	{
//	//		for (const auto& input : inputHardWareIdx)
//	//		{
//	//			if(InputDevice(input.hardWare) == InputDevice::D_Keyboard)
//	//			{
//	//				
//	//			}
//	//		}
//	//	}
//
//	//	case InputDevice::D_Mouse: break;
//	//	case InputDevice::D_Gamepad: break;
//	//	default:;
//	//	}
//	//}
//}

void powe::InputSettings::ParseHWMessages(const HardwareMessages& hwMessages)
{
	AxisKey inKey{};

	for (int i = 0; i < hwMessages.totalMessages; ++i)
	{
		const HardwareBus& hwBus{ hwMessages.hwMessages[i] };

		if (hwBus.inDevice == InputDevice::D_Keyboard)
		{
			const bool isKeyPressed{ WindowEvents(hwBus.eventId) == WindowEvents::KeyPressed ? true : false };
			inKey.scale = float(isKeyPressed);
			inKey.key = { hwBus.inDevice, std::get<KeyboardData>(hwBus.hData).keyCode };

			ValidateKeyState(inKey, isKeyPressed);
		}
		else if (hwBus.inDevice == InputDevice::D_Mouse)
		{
			bool isKeyPressed{};

			switch (WindowEvents(hwBus.eventId))
			{
			case WindowEvents::MouseWheelScrolled:

				isKeyPressed = true;
				inKey.key = { hwBus.inDevice,KeyType(MouseKey::MK_Middle) };
				inKey.scale = GetMouseData<MouseWheelDelta>(hwBus.hData);

				ValidateKeyState(inKey, isKeyPressed);

				m_ShouldRevalidateMouseValue = true;

				continue; // continue with the loop
			case WindowEvents::MouseButtonPressed:

				isKeyPressed = true;
				inKey.key = { hwBus.inDevice,GetMouseData<MouseCharKey>(hwBus.hData) };
				inKey.scale = 1.0f;

				ValidateKeyState(inKey, isKeyPressed);

				continue; // continue with the loop
			case WindowEvents::MouseButtonReleased:

				isKeyPressed = false;
				inKey.key = { hwBus.inDevice,GetMouseData<MouseCharKey>(hwBus.hData) };
				inKey.scale = 0.0f;

				ValidateKeyState(inKey, isKeyPressed);

				continue; // continue with the loop
			default: break;
			}

			// If we past this line then we know that this is a mouse move
			ValidateMouseDelta(GetMouseData<MousePos>(hwBus.hData));
			m_ShouldRevalidateMouseValue = true;
		}
	}


}

float powe::InputSettings::GetInputAxis(const std::string& axisName, uint8_t playerIndex) const
{
	try
	{
		const auto& axisMap{ m_AxisKeyMappings.at(axisName) };

		for (const auto& val : axisMap.keys)
		{
			const InputState& state{ m_MainKeyPool.at(val.key) };

			if (state.userIndex == playerIndex &&
				(state.keyEvent == InputEvent::IE_Down ||
					state.keyEvent == InputEvent::IE_Pressed))
			{
				return val.scale * state.axisValue;
			}
		}
	}
	catch (const std::exception& e)
	{
		std::string log{ e.what() };
		log.append(" Can't find given axis name, maybe you forgot to add it?");
		POWLOGERROR(log);
	}

	return 0.0f;
}

InputEvent powe::InputSettings::GetInputEvent(const std::string& actionName, uint8_t playerIndex) const
{
	try
	{
		const auto& axisMap{ m_AxisKeyMappings.at(actionName) };

		for (const auto& val : axisMap.keys)
		{
			const InputState& state{ m_MainKeyPool.at(val.key) };

			if (state.userIndex == playerIndex)
				return state.keyEvent;
		}
	}
	catch (const std::exception& e)
	{
		std::string log{ e.what() };
		log.append(" Can't find given action name, maybe you forgot to add it?");
		POWLOGERROR(log);
	}

	return InputEvent::IE_None;
}

void powe::InputSettings::AddInputEvent(const std::string& , const std::initializer_list<ActionKey>& )
{
	
}

bool powe::InputSettings::IsKeyBoardPressed(KeyType key)
{
	return sf::Keyboard::isKeyPressed(sf::Keyboard::Key(key));
}

void powe::InputSettings::ValidateKeyState(const AxisKey& key, bool isKeyPressed)
{
	const auto& itr{ m_MainKeyPool.find(key.key) };
	if (itr != m_MainKeyPool.end())
	{
		InputState& currentInputState{ itr->second };

		currentInputState.keyEvent = InterpretInputState(isKeyPressed, currentInputState.keyEvent);

		// Interpret Axis value
		if ((currentInputState.keyEvent == InputEvent::IE_Down) ||
			(currentInputState.keyEvent == InputEvent::IE_Pressed))
			currentInputState.axisValue = key.scale;
		else
			currentInputState.axisValue = 0.0f;
	}
}

InputEvent powe::InputSettings::InterpretInputState(bool isKeyPressed, const InputEvent& savedInputState)
{
	if (isKeyPressed)
	{
		switch (savedInputState)
		{

		case InputEvent::IE_Released:
		case InputEvent::IE_None:
			return InputEvent::IE_Pressed;

		case InputEvent::IE_Pressed:	return InputEvent::IE_Down;
		case InputEvent::IE_Down:		return savedInputState;
		}
	}
	else
	{
		switch (savedInputState)
		{
		case InputEvent::IE_Pressed:
		case InputEvent::IE_Down:
			return InputEvent::IE_Released;

		case InputEvent::IE_Released:   return InputEvent::IE_None;
		case InputEvent::IE_None:		return savedInputState;
		}
	}


	return InputEvent::IE_None;
}

void powe::InputSettings::ValidateMouseDelta(const MousePos& mousePos)
{
	// Mouse AxisX
	// TODO: Implement mouse axis movement
	const auto& itrMouseAxisX{ m_MainKeyPool.find(Key{ InputDevice::D_Mouse,KeyType(MouseKey::MK_AxisX) }) };
	if (itrMouseAxisX != m_MainKeyPool.end())
	{
		InputState& currentInputState{ itrMouseAxisX->second };
		currentInputState.axisValue = (currentInputState.axisValue - float(mousePos.relativePosX));
	}
}

//void powe::InputSettings::ParseMouseKey(const MouseData& key, uint8_t eventId, bool isKeyPressed)
//{
//}
//
//void powe::InputSettings::ParseMouseKey(const Key& key, bool isKeyPressed)
//{
//	const auto& itr{ m_MainKeyPool.find(key) };
//	if (itr != m_MainKeyPool.end())
//	{
//		InputState& currentInputState{ itr->second };
//
//		currentInputState.keyEvent = InterpretInputState(isKeyPressed, currentInputState.keyEvent);
//
//		// Interpret Axis value
//		if ((currentInputState.keyEvent == InputEvent::IE_Down) ||
//			(currentInputState.keyEvent == InputEvent::IE_Pressed))
//			currentInputState.axisValue = 1.0f;
//		else
//			currentInputState.axisValue = 0.0f;
//	}
//}

#endif

