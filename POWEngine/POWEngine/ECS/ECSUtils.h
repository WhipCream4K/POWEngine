#pragma once

#include "ECSTypes.h"
//#include <string>
//#include "POWEngine/Core/Components/BaseComponent.h"

namespace powe
{
    //static inline std::string ComponentIDToString(ComponentTypeID id)
    //{
    //	return std::to_string(id & ~(1u << SparseBitPos));
    //}

    static constexpr inline bool IsThisComponentSparse(ComponentTypeID id)
    {
        return id & int(ComponentFlag::Sparse);
    }

    static inline bool IsDigitExistInNumber(const std::vector<ComponentTypeID>& compIds,
                                            const std::unordered_set<ComponentTypeID>& digit)
    {
        for (const auto& id : compIds)
        {
            // this checks also take into account the hierarchy of the components
            const ComponentTypeID number{id & ~(SizeType(ComponentFlag::Count))}; // clearing the child of bit flag
            if (!digit.contains(number))
                return false;
        }

        return true;
    }

    static inline bool IsDigitExistInNumber(const ECSComponentMap<SizeType>& compIds,
                                            const std::unordered_set<ComponentTypeID>& digit)
    {
        for (const ComponentTypeID key : digit)
        {
            if (!compIds.contains(key))
                return false;
        }

        return true;
        //for (const auto& id : compIds)
        //{
        //	// this checks also take into account the hierarchy of the components
        //	const ComponentTypeID number{ id & ~(SizeType(ComponentFlag::Count)) }; // clearing the child of bit flag
        //	if (!digit.contains(number))
        //		return false;
        //}

        //return true;
    }

    static constexpr inline ComponentTypeID DiscardFlag(ComponentTypeID id)
    {
        return id & ~SizeType(ComponentFlag::Count);
    }

    //template <typename ...Args, typename = std::enable_if_t<(std::is_base_of_v<Component<Args>,Args> && ...)>>
    //std::unordered_set<ComponentTypeID> MakeSystemKeys()
    //{
    //    return {BaseComponent::GetId<Args>()...};
    //}

    
}
