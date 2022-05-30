#pragma once

#include "POWEngine/Core/Components/BaseComponent.h"

#include <SFML/Graphics.hpp>

namespace powe
{
	struct SFMLDebugRectangleComponent : public Component<SFMLDebugRectangleComponent>
	{
		sf::RectangleShape rectangle{};
		int drawOrder{};
	};
}


