#include "pch.h"
#include "InputSettings.h"
#include "POWEngine/Logger/LoggingService.h"

#if USE_SFML_WINDOW

#include <SFML/Window.hpp>

void powe::InputSettings::ParseWndMessages(
	const WindowMessages& winMessages,
	const std::array<WndMessageHWIdx, MinimumWindowEventCnt>& inputHardWareIdx)
{

	Key itrKey{};

	// KeyPool lookUp vs iterate KeyPool
	for (const auto& input : inputHardWareIdx)
	{
		// TODO: Maybe take care of out of range

		// maybe overflow operation uint8_t -> int
		const MessageBus msgBus{ winMessages.wndMessages[int(input.idx)] };

		switch (input.hardWare)
		{
		case InputDevice::D_Keyboard:
		{
			itrKey.inputDevice = InputDevice::D_Keyboard;

			// TODO: Take care of syskey
			const auto& sfKey{ std::any_cast<sf::Event::KeyEvent>(msgBus.data) };
			itrKey.keyCode = uint16_t(sfKey.code);

			ParseKeyBoardKey(msgBus.eventId, itrKey);

			break;
		}
		case InputDevice::D_Mouse:
		{
			try
			{
				itrKey.inputDevice = InputDevice::D_Mouse;

				const auto& sfMouse{ std::any_cast<sf::Event::MouseButtonEvent>(msgBus.data) };
				itrKey.keyCode = uint16_t(sfMouse.button);

				ParseMouseKey(msgBus.eventId, itrKey);
			}
			catch (const std::exception& some)
			{
				//POWLOGERROR(some.what());
				POWLOGINFO(some.what());
			}

			break;
		}
		case InputDevice::D_Gamepad:

			//TODO: Support the gamepad


			break;
		default:;
		}
	}

	//for (const auto& keyPool : m_MainKeyPool)
	//{
	//	switch (keyPool.first.inputDevice)
	//	{
	//	case InputDevice::D_Keyboard:
	//	{
	//		for (const auto& input : inputHardWareIdx)
	//		{
	//			if(InputDevice(input.hardWare) == InputDevice::D_Keyboard)
	//			{
	//				
	//			}
	//		}
	//	}

	//	case InputDevice::D_Mouse: break;
	//	case InputDevice::D_Gamepad: break;
	//	default:;
	//	}
	//}
}

bool powe::InputSettings::IsKeyBoardPressed(KeyType key)
{
	return sf::Keyboard::isKeyPressed(sf::Keyboard::Key(key));
}

void powe::InputSettings::ParseKeyBoardKey(uint8_t eventId, const Key& )
{
	try
	{
		switch (eventId)
		{
		case EventType::KeyPressed:



			break;
		case EventType::KeyReleased:

			break;
		default:;
		}
	}
	catch (const std::exception& )
	{

	}

}

void powe::InputSettings::ParseMouseKey(uint8_t , const Key& )
{

}

#endif

