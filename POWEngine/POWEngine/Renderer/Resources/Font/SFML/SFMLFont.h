#pragma once


#include "POWEngine/Renderer/Resources/Font/FontImpl.h"

#include <SFML/Graphics.hpp>

namespace powe
{
	class SFMLFont : public FontImpl
	{
	public:

		SFMLFont() = default;
		SFMLFont(const std::string& filePath);

		sf::Font& GetSFFont() { return m_Font; }

	private:

		sf::Font m_Font;
	};
}



