#include "pch.h"
#include "SFMLFont.h"

#include "POWEngine/Logger/LoggerUtils.h"

powe::SFMLFont::SFMLFont(const std::string& filePath)
	: m_Font()
{
	if (!m_Font.loadFromFile(filePath))
	{
		const std::string errMsg{ "SFML Font has failed to load" };
		POWLOGERROR(errMsg);
		throw std::runtime_error(errMsg.c_str());
	}
}
