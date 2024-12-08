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
            m_ECS->AddComponents(m_ID, std::forward<Args>(args)...);
        }

        EntityID GetID() const noexcept { return m_ID; }

    private:

        RefWrap<ECSManager> m_ECS;
        EntityID m_ID;
    };

}