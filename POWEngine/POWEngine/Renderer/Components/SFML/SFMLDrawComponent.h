#pragma once

#include "ISFMLDrawable.h"
#include "POWEngine/Core/Components/BaseComponent.h"

namespace powe
{
    struct SFMLDrawComponent :  Component<SFMLDrawComponent> ,  ISFMLDrawable
    {
        sf::Drawable* GetDrawable() override {return nullptr;}
        int drawOrder{};
        ComponentTypeID derivedTypeID{};
    };
}

