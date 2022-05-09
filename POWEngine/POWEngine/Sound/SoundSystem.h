#pragma once

#include "POWEngine/Services/Service.h"

namespace powe
{
	class SoundAPI;
	class SoundSystem : public Service
	{
	public:



	private:

		SharedPtr<SoundAPI> m_SoundAPI;
	};
}



