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
}
