#pragma once

#include "ECS/ComponentView.h"
#include "ECS/ECSTypes.h"

namespace powe
{
    class SceneQuery
    {
    public:
        
        // Query System
        template <typename... Args> requires (ComponentConcept<Args> && ...)
        static ComponentCollection<Args...> QueryComponents(DynamicBitsetRange<ComponentView>& views,Vector<ComponentID>& idCache)
        {
            idCache = MakeComponentRange<Args...>();
            const auto targetView{ FindComponentView(views,idCache) };
            return ComponentCollection<Args...>{&targetView};
        }

        static ComponentView& FindComponentView(DynamicBitsetRange<ComponentView>& views,const Vector<ComponentID>& compIDs)
        {
            return std::ranges::find_if(views,[&compIDs](auto& view)
            {
                return  IsArchetypeMatch(view.first,compIDs);
            })->second;
        }

        
    };
}
