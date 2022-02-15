
#include "pch.h"
#include "../Window.h"

#if USE_SFML_WINDOW

//#include <SFML/Graphics.hpp>

powe::Window::Window(uint32_t width, uint32_t height, const std::string& title)
	: m_WndHandle(sf::RenderWindow{ sf::VideoMode(width,height),title })
	, m_WndMessages()
	, m_Title(title)
	, m_Width(width)
	, m_Height(height)
{
}


const WindowMessages& powe::Window::PollWindowMessages(bool& shouldEarlyExit)
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
				//OnLostFocus();
				break;
			}
			case sf::Event::KeyPressed:
			case sf::Event::KeyReleased:
			{
				messageData.data = sfmlEvent.key;
				messageData.size = sizeof(sf::Event::KeyEvent);

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

			default: break;
			}

			messageData.eventId = uint8_t(sfmlEvent.type);

			m_WndMessages.wndMessages[messageCnt++] = messageData;
		}

		m_WndMessages.totalMessages = messageCnt;
	}

	return m_WndMessages;
}

void powe::Window::InternResize(uint32_t width, uint32_t height)
{
	m_WndHandle.setSize(sf::Vector2u(width, height));
}


#endif
