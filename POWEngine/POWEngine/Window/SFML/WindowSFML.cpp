
#include "pch.h"
#include "WindowSFML.h"
#include "POWEngine/Window/WindowEvents.h"
//#include "POWEngine/Core/Input/InputStruct.h"
#include <iostream>

#include "POWEngine/Core/Input/Key.h"

#if USE_SFML_WINDOW

powe::WindowSFML::WindowSFML(uint32_t width, uint32_t height, const std::string& title, OtherWindowParams others)
	: WindowImpl(width, height, title, others)
	, m_WndHandle(
		sf::VideoMode(width, height),
		sf::String{ title.c_str() },
		static_cast<sf::Uint32>(others[0]),
		reinterpret_cast<const sf::ContextSettings&>(others[sizeof(sf::Uint32)]))
{
}

powe::WindowSFML::WindowSFML(uint32_t width, uint32_t height, const std::string& title)
	: WindowImpl(width, height, title)
	, m_WndHandle(sf::VideoMode(width, height), sf::String{ title.c_str() })
{
}

const powe::WindowMessages& powe::WindowSFML::PollWindowMessages(bool& shouldEarlyExit, bool& shouldIgnoreInputs)
{
	m_WndMessages.totalMessages = 0;

	uint8_t messageCnt{};

	if (m_WndHandle.isOpen())
	{
		sf::Event sfmlEvent{};

		while (m_WndHandle.pollEvent(sfmlEvent))
		{
			if (messageCnt > MinimumWindowEventCnt)
				break;

			MessageBus messageData{};

			switch (sfmlEvent.type)
			{

			case sf::Event::Closed:
			{
				shouldEarlyExit = true;
				return m_WndMessages;
			}
			case sf::Event::GainedFocus:
			{
				break;
			}
			case sf::Event::LostFocus:
			{
				shouldIgnoreInputs = true;
				return m_WndMessages;
			}
			case sf::Event::KeyPressed:
			case sf::Event::KeyReleased:
			{
				//messageData.data = sfmlEvent.key;
				//messageData.size = sizeof(sf::Event::KeyEvent);

				KeyboardData keyboardData{ uint8_t(sfmlEvent.key.code) };
				keyboardData.sysKey = uint8_t(sfmlEvent.key.alt << int(KeyboardSysKey::KS_Alt) |
					sfmlEvent.key.control << int(KeyboardSysKey::KS_Ctrl) |
					sfmlEvent.key.shift << int(KeyboardSysKey::KS_Shift) |
					sfmlEvent.key.system << int(KeyboardSysKey::KS_System));

				messageData.data = keyboardData;

				break;
			}
			//case sf::Event::TextEntered:
			//{
			//	
			//	break;
			//}
			case sf::Event::Resized:
			{
				//messageData.data = sfmlEvent.size;
				//messageData.size = sizeof(sf::Event::SizeEvent);

				Resize(sfmlEvent.size.width, sfmlEvent.size.height);

				break;
			}
			case sf::Event::MouseButtonPressed:
			case sf::Event::MouseButtonReleased:
			{

				messageData.data = MouseKey(sfmlEvent.mouseButton.button);

				//messageData.data = sfmlEvent.mouseButton;
				//messageData.size = sizeof(sf::Event::MouseButtonEvent);

				break;
			}
			case sf::Event::MouseWheelScrolled:
			{
				messageData.data = MouseWheelDelta(sfmlEvent.mouseWheelScroll.delta);
				//messageData.data = sfmlEvent.mouseWheelScroll;
				//messageData.size = sizeof(sf::Event::MouseWheelScrollEvent);

				break;
			}
			case sf::Event::MouseMoved:
			{
				messageData.data = MousePos{ sfmlEvent.mouseMove.x,sfmlEvent.mouseMove.y };

				//messageData.data = sfmlEvent.mouseMove;
				//messageData.size = sizeof(sf::Event::MouseMoveEvent);

				break;
			}
			// TODO: Maybe support this using XInput
			//case sf::Event::TextEntered: break;
			//case sf::Event::MouseEntered: break;
			//case sf::Event::MouseLeft: break;
			//case sf::Event::JoystickButtonPressed: break;
			//case sf::Event::JoystickButtonReleased: break;
			//case sf::Event::JoystickMoved: break;
			//case sf::Event::JoystickConnected: break;
			//case sf::Event::JoystickDisconnected: break;
			//case sf::Event::TouchBegan: break;
			//case sf::Event::TouchMoved: break;
			//case sf::Event::TouchEnded: break;
			//case sf::Event::SensorChanged: break;
			//case sf::Event::Count: break;
			default: break;
			}

			messageData.eventId = uint8_t(sfmlEvent.type);

			m_WndMessages.wndMessages[messageCnt++] = messageData;
		}

		m_WndMessages.totalMessages = messageCnt;
	}

	return m_WndMessages;
}

const powe::HardwareMessages& powe::WindowSFML::PollHardwareMessages(bool& shouldEarlyExit, bool& shouldIgnoreInputs)
{
	m_HWMessages.totalMessages = 0;

	uint8_t messageCnt{};

	if (m_WndHandle.isOpen())
	{
		sf::Event sfmlEvent{};

		while (m_WndHandle.pollEvent(sfmlEvent))
		{
			if (messageCnt > MinimumWindowEventCnt)
				break;

			HardwareBus messageData{};

			switch (sfmlEvent.type)
			{

			case sf::Event::Closed:
			{
				shouldEarlyExit = true;
				return m_HWMessages;
			}
			case sf::Event::GainedFocus:
			{
				break;
			}
			case sf::Event::LostFocus:
			{
				shouldIgnoreInputs = true;
				return m_HWMessages;
			}
			case sf::Event::KeyPressed:
			{
				KeyboardData keyboardData{ uint8_t(sfmlEvent.key.code) };
				keyboardData.sysKey = uint8_t(sfmlEvent.key.alt << int(KeyboardSysKey::KS_Alt) |
					sfmlEvent.key.control << int(KeyboardSysKey::KS_Ctrl) |
					sfmlEvent.key.shift << int(KeyboardSysKey::KS_Shift) |
					sfmlEvent.key.system << int(KeyboardSysKey::KS_System));

				messageData.hData = keyboardData;
				messageData.eventId = uint8_t(WindowEvents::KeyPressed);
				messageData.inDevice = InputDevice::D_Keyboard;

				m_HWMessages.hwMessages[messageCnt++] = messageData;

				break;
			}
			case sf::Event::KeyReleased:
			{
				KeyboardData keyboardData{ uint8_t(sfmlEvent.key.code) };
				keyboardData.sysKey = uint8_t(sfmlEvent.key.alt << int(KeyboardSysKey::KS_Alt) |
					sfmlEvent.key.control << int(KeyboardSysKey::KS_Ctrl) |
					sfmlEvent.key.shift << int(KeyboardSysKey::KS_Shift) |
					sfmlEvent.key.system << int(KeyboardSysKey::KS_System));

				messageData.hData = keyboardData;
				messageData.eventId = uint8_t(WindowEvents::KeyReleased);
				messageData.inDevice = InputDevice::D_Keyboard;

				m_HWMessages.hwMessages[messageCnt++] = messageData;

				break;
			}
			case sf::Event::Resized:
			{
				Resize(sfmlEvent.size.width, sfmlEvent.size.height);
				break;
			}
			case sf::Event::MouseButtonPressed:
			{
				messageData.hData = MouseCharKey(sfmlEvent.mouseButton.button);
				messageData.eventId = uint8_t(WindowEvents::MouseButtonPressed);
				messageData.inDevice = InputDevice::D_Mouse;

				m_HWMessages.hwMessages[messageCnt++] = messageData;

				break;
			}
			case sf::Event::MouseButtonReleased:
			{
				messageData.hData = MouseCharKey(sfmlEvent.mouseButton.button);
				messageData.eventId = uint8_t(WindowEvents::MouseButtonReleased);
				messageData.inDevice = InputDevice::D_Mouse;

				m_HWMessages.hwMessages[messageCnt++] = messageData;

				break;
			}
			case sf::Event::MouseWheelScrolled:
			{
				messageData.hData = MouseWheelDelta(sfmlEvent.mouseWheelScroll.delta);
				messageData.eventId = uint8_t(WindowEvents::MouseWheelScrolled);
				messageData.inDevice = InputDevice::D_Mouse;

				m_HWMessages.hwMessages[messageCnt++] = messageData;

				break;
			}
			case sf::Event::MouseMoved:
			{
				messageData.hData = MousePos{ sfmlEvent.mouseMove.x,sfmlEvent.mouseMove.y };
				messageData.eventId = uint8_t(WindowEvents::MouseMoved);
				messageData.inDevice = InputDevice::D_Mouse;

				m_HWMessages.hwMessages[messageCnt++] = messageData;

				break;
			}
			// TODO: Implement gamepad using XInput

			default: break;
			}
		}

		m_HWMessages.totalMessages = messageCnt;
	}

	return m_HWMessages;
}

void powe::WindowSFML::Resize(uint32_t width, uint32_t height)
{
	m_WndHandle.setSize({ width,height });
}

void powe::WindowSFML::SetTitle(const std::string& title)
{
	m_WndHandle.setTitle(sf::String{ title.c_str() });
}

powe::WindowSFML::~WindowSFML() = default;


#endif


