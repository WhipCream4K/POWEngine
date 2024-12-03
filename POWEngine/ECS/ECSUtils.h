#pragma once

#include "ECSTypes.h"
#include "ComponentIDGen.h"

namespace powe
{
    template<ComponentConcept... Args>
    Vector<ComponentID> MakeComponentRange()
    {
        Vector<ComponentID> key{};
        (key.emplace_back(ComponentIDGen::Get<Args>()), ...);
        return key;
    }

    template<ComponentConcept... Args>
    void MakeComponentRange(Vector<ComponentID>& key)
    {
        (key.emplace_back(ComponentIDGen::Get<Args>()), ...);
    }


    template<is_tuple Tuple,size_t ...Is>
    Vector<ComponentID> MakeComponentRange(std::index_sequence<Is...>)
    {
        Vector<ComponentID> key{};
        // (key.emplace_back(std::tuple_element_t<Is, std::decay_t<Tuple>>(tuple)>), ...);
        (key.emplace_back(ComponentIDGen::Get<std::tuple_element_t<Is,Tuple>>()), ...);
        return key;
    }
    
    template<is_tuple Tuple>
    void MakeComponentRange()
    {
        return MakeComponentRange(std::make_index_sequence<std::tuple_size_v<std::decay_t<Tuple>>>{});
    }
    



    inline DynamicBitSet MakeArchetypeKey(const Vector<ComponentID>& compIDs)
    {
        DynamicBitSet key(ComponentIDGen::Size());
        for (const auto& id : compIDs)
        {
            key[id] = true;
        }
        return key;
    }
    
    inline bool IsArchetypeMatch(const DynamicBitSet& archetypeKey, const Vector<ComponentID>& queryKey)
    {
        // might throw watch out
        return std::ranges::all_of(queryKey, [&archetypeKey](const ComponentID& id) { return archetypeKey[id]; });
    }


}
