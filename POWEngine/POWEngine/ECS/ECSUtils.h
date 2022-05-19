#pragma once

#include "ECSTypes.h"
#include <string>

namespace powe
{
	//static inline std::string ComponentIDToString(ComponentTypeID id)
	//{
	//	return std::to_string(id & ~(1u << SparseBitPos));
	//}

	static inline bool IsThisComponentSparse(ComponentTypeID id)
	{
		return id & int(ComponentFlag::Sparse);
	}

	static inline bool IsDigitExistInNumber(const std::vector<ComponentTypeID>& compIds,
		const std::unordered_set<ComponentTypeID>& digit)
	{
		for (const auto& id : compIds)
		{
			// this checks also take into account the hierarchy of the components
			const ComponentTypeID number{ id & ~(SizeType(ComponentFlag::Count)) }; // clearing the child of bit flag
			if (!digit.contains(number))
				return false;
		}

		return true;
	}
}
