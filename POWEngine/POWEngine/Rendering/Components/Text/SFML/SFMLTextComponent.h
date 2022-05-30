#pragma once

#include "POWEngine/Core/Components/BaseComponent.h"


#include <SFML/Graphics.hpp>

namespace powe
{
	struct SFMLTextComponent : Component<SFMLTextComponent>
	{
		sf::Text text{};
		int renderOrder{};
	};
}
