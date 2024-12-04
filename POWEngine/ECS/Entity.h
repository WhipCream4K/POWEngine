#pragma once

#include "ECSTypes.h"
#include "ECSManager.h"

namespace powe 
{
    class ECSManager;
    class Entity final
    {
    public:

        Entity(ECSManager& manager);

        ~Entity() = default;

        template<typename... Args> requires (ComponentConcept<Args> && ...)
        void Add(Args&&... args) const noexcept
        {
            // Check if this entity already has this component
            const auto archetype{ m_ECS->GetArchetypeFrom(m_ID) };
            if(archetype == nullptr)
            {
                // If not, create a new archetype with this component
                const auto newArchetype{ m_ECS->GetOrCreateArchetype<Args...>() };
                newArchetype->emplace_back(m_ID, std::forward<Args>(args)...);
            }
            else
            {
                // If so, add this component to the existing archetype
            }
        }

        EntityID GetID() const noexcept;

    private:

        RefWrap<ECSManager> m_ECS;
        EntityID m_ID;
    };

}