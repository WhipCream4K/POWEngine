#pragma once

#include <fmod.hpp>
#include <fmod_errors.h>
#include <stdexcept>

namespace powe
{
	static void inline FMODErrorCheck(FMOD_RESULT result)
	{
		if (result != FMOD_OK)
			throw std::runtime_error(FMOD_ErrorString(result));
	}
}