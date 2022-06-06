#pragma once

#include "POWEngine/Rendering/Resources/Texture/TextureImpl.h"

#if USE_SFML_RENDERER

#include <SFML/Graphics.hpp>

namespace powe
{
	class SFMLTexture :
		public TextureImpl
	{
	public:

		SFMLTexture();
		SFMLTexture(const std::string& filePath);
		SFMLTexture(const SFMLTexture&) = delete;
		SFMLTexture& operator=(const SFMLTexture&) = delete;
		SFMLTexture(SFMLTexture&&) = delete;
		SFMLTexture& operator=(SFMLTexture&&) = delete;
		void SetRepeated(bool isRepeated) override;
		sf::Texture& GetSFTexture() { return m_Texture; }

	private:
		
		sf::Texture m_Texture{};
	};
}

#endif


