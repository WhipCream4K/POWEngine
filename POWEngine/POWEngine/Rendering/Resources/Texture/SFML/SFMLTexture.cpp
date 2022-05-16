#include "pch.h"
#include "SFMLTexture.h"
#include "POWEngine/Logger/LoggerUtils.h"

#if USE_SFML_RENDERER

powe::SFMLTexture::SFMLTexture()
	: m_Texture()
{
}

powe::SFMLTexture::SFMLTexture(const std::string& filePath)
	: m_Texture()
{
	if(!m_Texture.loadFromFile(filePath))
	{
		const std::string errMsg{ "Load SFML Texture failed" };
		POWLOGERROR(errMsg);
		throw std::runtime_error(errMsg.c_str());
	}
}

#endif
