#pragma once

#include "ECSTypes.h"

namespace powe 
{
    class ECSManager;
    class Entity final
    {
    public:

        Entity(ECSManager& manager);

        ~Entity() = default;

        template<typename... Args> requires (ComponentConcept<Args> && ...)
        constexpr void Add(Args&&... args) const noexcept
        {
                              
        }

        EntityID GetID() const noexcept;

    private:

        RefWrap<ECSManager> m_ECS;
        EntityID m_ID;
    };

}