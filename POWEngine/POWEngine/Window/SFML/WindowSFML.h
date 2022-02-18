#pragma once

#include "POWEngine/Window/WindowImpl.h"

#if USE_SFML_WINDOW

#include <SFML/Graphics.hpp>

namespace powe
{

	class WindowSFML : public WindowImpl
	{

	public:

		WindowSFML(uint32_t width, uint32_t height, const std::string& title, OtherWindowParams others);
		WindowSFML(uint32_t width, uint32_t height, const std::string& title);
		const WindowMessages& PollWindowMessages(bool& shouldEarlyExit) override;
		void Resize(uint32_t width, uint32_t height) override;
		void SetTitle(const std::string& title) override;

		~WindowSFML() override;

	private:

		sf::RenderWindow m_WndHandle;
	};
}

#endif



