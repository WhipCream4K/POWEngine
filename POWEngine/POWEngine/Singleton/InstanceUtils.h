#pragma once

#include "POWEngine/Core/CustomTypes.h"

namespace powe
{
	template<typename T>
	static SharedPtr<T> Instance()
	{
		return T::GetInstance();
	}
}
