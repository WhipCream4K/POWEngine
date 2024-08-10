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

    static DynamicBitSet MakeArchetypeKey(const Vector<ComponentID>& compIDs)
    {
        DynamicBitSet key(compIDs.size());
        for(const auto& id : compIDs)
        {
            key[id] = true;
        }
        return key;
    }

    static bool IsArchetypeMatch(const DynamicBitSet& archetypeKey, const Vector<ComponentID>& queryKey)
    {
        return std::ranges::all_of(queryKey, [&archetypeKey](const ComponentID& id) { return archetypeKey[id]; });
    }


}
