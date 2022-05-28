
#include "pch.h"
#include "SFMLWindow.h"
#include "POWEngine/Window/WindowEvents.h"
#include "POWEngine/Core/Input/Key.h"
#include "POWEngine/Core/Input/ListsOfKeys.h"

#if USE_SFML_WINDOW

powe::SFMLWindow::SFMLWindow(uint32_t width, uint32_t height, const std::string& title, OtherWindowParams others)
	: WindowImpl(width, height, title, others)
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
				messageData.hData = MouseData{ MouseCharKey(sfmlEvent.mouseButton.button) };
				messageData.eventId = uint8_t(WindowEvents::MouseButtonPressed);
				messageData.inDevice = InputDevice::D_Mouse;

				m_HWMessages.hwMessages[messageCnt++] = messageData;

				break;
			}
			case sf::Event::MouseButtonReleased:
			{
				messageData.hData = MouseData{ MouseCharKey(sfmlEvent.mouseButton.button) };
				messageData.eventId = uint8_t(WindowEvents::MouseButtonReleased);
				messageData.inDevice = InputDevice::D_Mouse;

				m_HWMessages.hwMessages[messageCnt++] = messageData;

				break;
			}
			case sf::Event::MouseWheelScrolled:
			{
				messageData.hData = MouseData{ MouseCharKey(MouseKey::MK_Middle),MouseWheelDelta(sfmlEvent.mouseWheelScroll.delta)};
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

				MousePos mousePos = MousePos{
					m_DeltaMousePos.x,
					m_DeltaMousePos.y,
					sfmlEvent.mouseMove.x,
					sfmlEvent.mouseMove.y };

				messageData.hData = MouseData{ MouseCharKey(MouseKey::MK_None),mousePos };

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

void powe::SFMLWindow::PollHardwareMessages(
	HardwareMessages& hardwareMessages, 
	bool& shouldEarlyExit,
	bool& shouldIgnoreInputs)
{
	int messageCnt{hardwareMessages.totalMessages};

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
				return;
			}
			case sf::Event::GainedFocus:
			{
				break;
			}
			case sf::Event::LostFocus:
			{
				shouldIgnoreInputs = true;
				return;
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

				hardwareMessages.hwMessages[messageCnt++] = messageData;

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

				hardwareMessages.hwMessages[messageCnt++] = messageData;

				break;
			}
			case sf::Event::Resized:
			{
				Resize(sfmlEvent.size.width, sfmlEvent.size.height);
				break;
			}
			case sf::Event::MouseButtonPressed:
			{
				messageData.hData = MouseData{ MouseCharKey(sfmlEvent.mouseButton.button) };
				messageData.eventId = uint8_t(WindowEvents::MouseButtonPressed);
				messageData.inDevice = InputDevice::D_Mouse;

				hardwareMessages.hwMessages[messageCnt++] = messageData;

				break;
			}
			case sf::Event::MouseButtonReleased:
			{
				messageData.hData = MouseData{ MouseCharKey(sfmlEvent.mouseButton.button) };
				messageData.eventId = uint8_t(WindowEvents::MouseButtonReleased);
				messageData.inDevice = InputDevice::D_Mouse;

				hardwareMessages.hwMessages[messageCnt++] = messageData;

				break;
			}
			case sf::Event::MouseWheelScrolled:
			{
				messageData.hData = MouseData{ MouseCharKey(MouseKey::MK_Middle),MouseWheelDelta(sfmlEvent.mouseWheelScroll.delta) };
				messageData.eventId = uint8_t(WindowEvents::MouseWheelScrolled);
				messageData.inDevice = InputDevice::D_Mouse;

				hardwareMessages.hwMessages[messageCnt++] = messageData;

				break;
			}
			case sf::Event::MouseMoved:
			{
				messageData.eventId = uint8_t(WindowEvents::MouseMoved);
				messageData.inDevice = InputDevice::D_Mouse;

				m_DeltaMousePos.x = float(sfmlEvent.mouseMove.x - m_MousePosLastPoll.x);
				m_DeltaMousePos.y = float(m_MousePosLastPoll.y - sfmlEvent.mouseMove.y);

				MousePos mousePos = MousePos{
					m_DeltaMousePos.x,
					m_DeltaMousePos.y,
					sfmlEvent.mouseMove.x,
					sfmlEvent.mouseMove.y };

				messageData.hData = MouseData{ MouseCharKey(MouseKey::MK_None),mousePos };

				m_MousePosLastPoll.x = sfmlEvent.mouseMove.x;
				m_MousePosLastPoll.y = sfmlEvent.mouseMove.y;

				hardwareMessages.hwMessages[messageCnt++] = messageData;

				break;
			}
			// TODO: Implement gamepad using XInput

			default: break;
			}
		}

		hardwareMessages.totalMessages = messageCnt;
	}
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

powe::SFMLWindow::~SFMLWindow()
{
	m_WndHandle.close();
}


#endif


