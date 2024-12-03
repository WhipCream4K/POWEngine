#pragma once

#include "ECS/ComponentView.h"
#include "ECS/ECSTypes.h"

namespace powe
{
    // Manages all the queries happening in the scene during update
    class SceneQuery
    {
    public:
        
        // Query System
        // template <typename... Args> requires (ComponentConcept<Args> && ...)
        // ComponentCollection<Args...> QueryComponents()
        // {
        //     m_CacheQueryIDs = MakeComponentRange<Args...>();
        //     const auto targetView{ FindComponentView(m_CacheQueryIDs) };
        //     return ComponentCollection<Args...>{&targetView};
        // }

    private:
        
        // ComponentView& FindComponentView(const Vector<ComponentID>& compIDs)
        // {
        //     return std::ranges::find_if(m_CachedComponentViews,[&compIDs](auto& view)
        //     {
        //         return  IsArchetypeMatch(view.first,compIDs);
        //     })->second;
        // }

        // DynamicBitsetRange<ComponentView> m_CachedComponentViews;
        // Vector<ComponentID> m_CacheQueryIDs;
    };
}
