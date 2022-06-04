#pragma once

#include "POWEngine/Math/Math.h"

inline bool IsOutOfBound(int direction, float pos, float against)
{
	if (direction > 0)
	{
		if (pos > against)
			return true;
	}
	else
	{
		if (pos < against)
			return true;
	}

	return false;
}

inline bool IsAligned(float posY, float againstY, float epsilon)
{
	if (abs(posY - againstY) <= epsilon)
		return true;

	return false;
}