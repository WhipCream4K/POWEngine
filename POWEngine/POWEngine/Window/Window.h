#pragma once

#include "../Core/CustomTypes.h"

#include <SFML/Graphics.hpp>
using WindowHandle = sf::RenderWindow;
#if USE_SFML_WINDOW


#endif

constexpr int MinimumWindowEventCnt = 23;

struct WindowMessages
{
	std::array<Any, MinimumWindowEventCnt> wndMessages{};
	int totalMessages{};
};

namespace powe
{
	class Window
	{
	public:

		Window(uint32_t width, uint32_t height, const std::string& title);
		const WindowMessages& PollWindowMessages(bool& shouldEarlyExit);

		void Resize(uint32_t width, uint32_t height);

	protected:

		void InternResize(uint32_t width, uint32_t height);

	private:

		WindowHandle m_WndHandle;
		WindowMessages m_WndMessages;

		std::string m_Title;
		uint32_t m_Width;
		uint32_t m_Height;
	};
}



