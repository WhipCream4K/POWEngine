#pragma once

#include "POWEngine/Core/Input/InputSettings.h"

namespace powe
{
	class WorldEntity final
	{
	public:

		WorldEntity();
		WorldEntity(const WorldEntity&) = delete;
		WorldEntity& operator=(const WorldEntity&) = delete;
		~WorldEntity() = default;


		InputSettings& GetInputSettings() { return  m_InputSettings; }

	private:

		InputSettings m_InputSettings;
	};
}


