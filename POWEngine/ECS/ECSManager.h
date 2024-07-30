#pragma once

#include "ECSTypes.h"
#include "Archetype.h"

namespace powe
{
	class ECSManager final
	{
	public:

		EntityID CreateEntity() { return m_CurrentEntityID++; }
		
		

	private:

		std::atomic<EntityID> m_CurrentEntityID{};

	};
}

