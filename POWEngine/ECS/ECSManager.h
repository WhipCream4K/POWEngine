#pragma once

#include "ECSTypes.h"
#include "Archetype.h"
#include "EntityView.h"

namespace powe
{
	template<ComponentConcept... Args>
	std::string GetArchetypeKey()
	{
		return std::string(typeid(Args).name() + ...);
	}

	class ECSManager final
	{
	public:

		EntityID CreateEntity() { return m_CurrentEntityID++; }

		template<ComponentConcept... Args>
		EntityID CreateEntity(Args&&... args)
		{
			Archetype<Args...>& archetype{ GetArchetype<Args...>() };
			EntityID newID{m_CurrentEntityID++};
			archetype.emplace_back(newID,std::forward<Args>(args)...);
			return newID;
		}
		
		template<ComponentConcept... Args>
		Archetype<Args...>& GetArchetype();
		

	private:

		UnOrderedMap<std::string, UniquePtr<void>> m_Archetypes;
		std::atomic<EntityID> m_CurrentEntityID{};

	};

	template<ComponentConcept ...Args>
	inline Archetype<Args...>& ECSManager::GetArchetype()
	{
		const std::string archetypeKey{ GetArchetypeKey<Args...>()};
		if (auto it = m_Archetypes.find(archetypeKey); it != m_Archetypes.end())
		{
			return *std::static_pointer_cast<Archetype<Args...>>(it->second);
		}

		Archetype<Args...>* archetype{ std::make_unique<Archetype<Args...>>() };
		m_Archetypes[archetypeKey] = std::move(archetype);

		return *m_Archetypes[archetypeKey];
	}
}

