#pragma once

#include "POWEngine/Core/Components/BaseComponent.h"

// put a pod as a component to use as a render buffer here

#if USE_SFML_RENDERER

#include <SFML/Graphics.hpp>

struct SFMLSpriteComponent : powe::Component<SFMLSpriteComponent>
{
	sf::Sprite sprite{};
	sf::RenderStates renderStates{};
};

#endif