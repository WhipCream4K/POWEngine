#include "pch.h"
#include "Font.h"


#ifdef USE_SFML_RENDERER
#include "SFML/SFMLFont.h"
using FontType = powe::SFMLFont;
#endif


powe::Font::Font(const std::string& filePath)
	: m_FontImpl(std::make_unique<FontType>(filePath))
{
}

powe::Font::~Font() = default;
