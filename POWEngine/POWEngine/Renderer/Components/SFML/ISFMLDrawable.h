#pragma once

namespace sf
{
    class Drawable;
}

namespace powe
{
    class ISFMLDrawable
    {
    public:
        ISFMLDrawable() = default;
        virtual ~ISFMLDrawable() = default;
        virtual sf::Drawable* GetDrawable() = 0;
    };
}

