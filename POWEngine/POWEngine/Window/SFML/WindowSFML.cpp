
#include "pch.h"
#include "WindowSFML.h"

#include <iostream>

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
				//OnGainFocus();
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
				messageData.data = sfmlEvent.key;
				messageData.size = sizeof(sf::Event::KeyEvent);

				//std::cout << "Type Here : " << sfmlEvent.key.code << '\n';

				break;
			}
			case sf::Event::Resized:
			{
				messageData.data = sfmlEvent.size;
				messageData.size = sizeof(sf::Event::SizeEvent);

				Resize(sfmlEvent.size.width, sfmlEvent.size.height);
				//OnResize(uint32_t(sfmlEvent.size.width), uint32_t(sfmlEvent.size.height));

				break;
			}
			case sf::Event::MouseButtonPressed:
			case sf::Event::MouseButtonReleased:
			{

				messageData.data = sfmlEvent.mouseButton;
				messageData.size = sizeof(sf::Event::MouseButtonEvent);

				break;
			}
			case sf::Event::MouseWheelScrolled:
			{
				messageData.data = sfmlEvent.mouseWheelScroll;
				messageData.size = sizeof(sf::Event::MouseWheelScrollEvent);

				break;
			}
			case sf::Event::MouseMoved:
			{
				messageData.data = sfmlEvent.mouseMove;
				messageData.size = sizeof(sf::Event::MouseMoveEvent);

				break;
			}
			// TODO: Maybe support this
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
			//default: break;
			}

			messageData.eventId = uint8_t(sfmlEvent.type);

			m_WndMessages.wndMessages[messageCnt++] = messageData;
		}

		m_WndMessages.totalMessages = messageCnt;
	}

	return m_WndMessages;
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


