#pragma once

#include <SFML/Graphics/Color.hpp>

namespace sf
{
    static sf::Color ConvertToSFColor(const glm::uvec4& color)
    {
        sf::Color sfColor{};
        sfColor.r = sf::Uint8(color.r);
        sfColor.g = sf::Uint8(color.g);
        sfColor.b = sf::Uint8(color.b);
        sfColor.a = sf::Uint8(color.a);

        return sfColor;
    }
}
