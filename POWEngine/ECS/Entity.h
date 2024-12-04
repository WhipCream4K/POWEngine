#pragma once

#include "ECSTypes.h"
#include "ECSManager.h"

namespace powe 
{
    class ECSManager;
    class Entity final
    {
    public:

        Entity(ECSManager& manager) noexcept;

        ~Entity();

        template<typename... Args> requires (ComponentConcept<Args> && ...)
        void Add(Args&&... args) const noexcept
        {
            // Check if this entity already has this component
            const auto archetype{ m_ECS->GetArchetypeFrom(m_ID) };
            if(archetype == nullptr)
            {
                auto addedArchetype{[id = m_ID, ...cArgs = std::forward<Args>(args)](ECSManager& manager) -> void {
                
                    
                }};

                m_ECS->SumbitLateAction(std::move(addedArchetype));
            }
            else
            {
                // If so, add this component to the existing archetype
                
            }
        }

        EntityID GetID() const noexcept { return m_ID; }

    private:

        RefWrap<ECSManager> m_ECS;
        EntityID m_ID;
    };

}