#pragma once

#include "ECSTypes.h"
#include "ComponentIDGen.h"

namespace powe
{
    template<ComponentConcept... Args>
    [[nodiscard]] Set<ComponentID> MakeComponentSet()
    {
        Set<ComponentID> key{};
        (key.emplace(ComponentIDGen::Get<Args>()), ...);
        return key;
    }

    template<ComponentConcept... Args>
    constexpr void MakeComponentSet(Set<ComponentID>& key)
    {
        (key.emplace(ComponentIDGen::Get<Args>()), ...);
    }

    template<is_tuple Tuple,size_t ...Is>
    [[nodiscard]] Set<ComponentID> MakeComponentSet(std::index_sequence<Is...>)
    {
        Set<ComponentID> key{};
        (key.emplace(ComponentIDGen::Get<std::tuple_element_t<Is,Tuple>>()), ...);
        return key;
    }
    
    template<is_tuple Tuple>
    [[nodiscard]] Set<ComponentID> MakeComponentSet()
    {
        return MakeComponentSet(std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>{});
    }

    template<typename... Args>
    [[nodiscard]] Vector<ComponentID> MakeComponentVec()
    {
        Vector<ComponentID> key{};
        (key.emplace_back(ComponentIDGen::Get<Args>()), ...);
        return key;
    }


    [[maybe_unused]] static inline bool IsArchetypeMatch(const Set<ComponentID>& archetypeKey, const Set<ComponentID>& queryKey)
    {
       return std::includes(queryKey.begin(), queryKey.end(), archetypeKey.begin(), archetypeKey.end());
    }    
}
