#pragma once

#include "POWEngine/Core/Components/BaseComponent.h"

#include <SFML/Graphics.hpp>

namespace powe
{
	struct SFMLSpriteComponent : Component<SFMLSpriteComponent>
	{
		sf::Sprite sprite{};
		sf::RenderStates renderStates{};
	};
}

