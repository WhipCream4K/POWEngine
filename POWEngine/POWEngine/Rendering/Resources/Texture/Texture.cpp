#include "pch.h"
#include "Texture.h"

#if USE_SFML_RENDERER
#include "SFML/SFMLTexture.h"
using TextureType = powe::SFMLTexture;
#endif

powe::Texture::Texture(const std::string& filePath)
	: m_TextureImpl(std::make_unique<TextureType>(filePath))
{
}

powe::Texture::~Texture() = default;
