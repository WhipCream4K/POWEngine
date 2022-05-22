
#include "pch.h"
#include "SFMLWindow.h"
#include "POWEngine/Window/WindowEvents.h"
#include "POWEngine/Core/Input/Key.h"

#if USE_SFML_WINDOW

powe::SFMLWindow::SFMLWindow(uint32_t width, uint32_t height, const std::string& title, OtherWindowParams others)
	: WindowImpl(width, height, title, others)
	, m_WndMessages()
	, m_HWMessages()
	, m_WndHandle(
		sf::VideoMode(width, height),
		sf::String{ title.c_str() },
		static_cast<sf::Uint32>(others[0]),
		reinterpret_cast<const sf::ContextSettings&>(others[sizeof(sf::Uint32)]))
	, m_MousePosLastPoll()
	, m_ClearColor(0, 0, 0, 255)
	, m_DeltaMousePos()
{
	const auto mousePos = sf::Mouse::getPosition(m_WndHandle);
	m_MousePosLastPoll.x = mousePos.x;
	m_MousePosLastPoll.y = mousePos.y;
}

powe::SFMLWindow::SFMLWindow(uint32_t width, uint32_t height, const std::string& title)
	: WindowImpl(width, height, title)
	, m_WndMessages()
	, m_HWMessages()
	, m_WndHandle(sf::VideoMode(width, height), sf::String{ title.c_str() })
	, m_MousePosLastPoll()
	, m_ClearColor(0, 0, 0, 255)
	, m_DeltaMousePos()
{
	const auto mousePos = sf::Mouse::getPosition(m_WndHandle);
	m_MousePosLastPoll.x = mousePos.x;
	m_MousePosLastPoll.y = mousePos.y;
}

const powe::WindowMessages& powe::SFMLWindow::PollWindowMessages(bool& shouldEarlyExit, bool& shouldIgnoreInputs)
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
				messageData.eventId = uint8_t(WindowEvents::MouseMoved);

				m_DeltaMousePos.x = float(sfmlEvent.mouseMove.x - m_MousePosLastPoll.x);
				m_DeltaMousePos.y = float(m_MousePosLastPoll.y - sfmlEvent.mouseMove.y);

				messageData.data = MousePos{
					m_DeltaMousePos.x,
					m_DeltaMousePos.y,
					sfmlEvent.mouseMove.x,
					sfmlEvent.mouseMove.y };

				m_MousePosLastPoll.x = sfmlEvent.mouseMove.x;
				m_MousePosLastPoll.y = sfmlEvent.mouseMove.y;

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

const powe::HardwareMessages& powe::SFMLWindow::PollHardwareMessages(bool& shouldEarlyExit, bool& shouldIgnoreInputs)
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
				messageData.eventId = uint8_t(WindowEvents::MouseMoved);
				messageData.inDevice = InputDevice::D_Mouse;

				m_DeltaMousePos.x = float(sfmlEvent.mouseMove.x - m_MousePosLastPoll.x);
				m_DeltaMousePos.y = float(m_MousePosLastPoll.y - sfmlEvent.mouseMove.y);

				messageData.hData = MousePos{
					m_DeltaMousePos.x,
					m_DeltaMousePos.y,
					sfmlEvent.mouseMove.x,
					sfmlEvent.mouseMove.y };

				m_MousePosLastPoll.x = sfmlEvent.mouseMove.x;
				m_MousePosLastPoll.y = sfmlEvent.mouseMove.y;

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

void powe::SFMLWindow::Resize(uint32_t width, uint32_t height)
{
	m_WndHandle.setSize({ width,height });
}

void powe::SFMLWindow::SetTitle(const std::string& title)
{
	m_WndHandle.setTitle(sf::String{ title.c_str() });
}

const glm::uvec2& powe::SFMLWindow::GetRelativeMousePos() const
{
	return m_MousePosLastPoll;
}

void powe::SFMLWindow::ClearWindow()
{
	sf::Uint8 r{ uint8_t(m_ClearColor.x) };
	sf::Uint8 g{ uint8_t(m_ClearColor.y) };
	sf::Uint8 b{ uint8_t(m_ClearColor.z) };
	sf::Uint8 a{ uint8_t(m_ClearColor.w) };

	m_WndHandle.clear({ r,g,b,a });
}

void powe::SFMLWindow::SetClearColor(const glm::uvec4& color)
{
	m_ClearColor = color;
}

void powe::SFMLWindow::Display()
{
	m_WndHandle.display();
}

const glm::uvec4& powe::SFMLWindow::GetClearColor() const
{
	return m_ClearColor;
}

powe::SFMLWindow::~SFMLWindow() = default;


#endif


