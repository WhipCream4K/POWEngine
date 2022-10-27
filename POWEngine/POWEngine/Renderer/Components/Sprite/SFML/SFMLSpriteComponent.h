#pragma once

#include <SFML/Graphics.hpp>

namespace powe
{
	struct SFMLSpriteComponent : public Component<SFMLSpriteComponent>
	{
		SFMLSpriteComponent() = default;

		sf::Sprite sprite{};
		int drawOrder{};
	};
}

