#include "pch.h"
#include "ECSManager.h"

powe::ECSManager::ECSManager(std::pmr::memory_resource* memResource)
	: m_Archetypes{ memResource }
	, m_MemResource(memResource)
{
}

void powe::ECSManager::GetArchetypes(const Vector<ComponentID>& query, Vector<IArchetype*>& outArchetypes) const
{
	for(const auto& [archetypeKey, archetypes] : m_Archetypes)
	{
		if(IsArchetypeMatch(archetypeKey, query))
		{
			for(const auto& archetype : archetypes)
			{
				outArchetypes.emplace_back(archetype.get());
			}
		}
	}
}

powe::IArchetype* powe::ECSManager::GetArchetype(const Vector<ComponentID>& compIDs) const
{
	for(const auto& [archetypeKey, archetypes] : m_Archetypes)
	{
		if(IsArchetypeMatch(archetypeKey, compIDs))
		{
			return archetypes.get();
		}
	}

	return nullptr;
}

bool powe::ECSManager::IsContainsArchetype(const Vector<ComponentID>& compIDs) const
{
	return std::ranges::any_of(m_Archetypes, [&compIDs](const auto& pair)
	{
		return IsArchetypeMatch(pair.first, compIDs);
	});
}

void powe::ECSManager::InsertArchetype(const Vector<ComponentID>& compIDs, UniquePtr<IArchetype>&& archetype)
{
	const auto key{ MakeArchetypeKey(compIDs) };
	m_Archetypes.emplace_back(std::make_pair(key, std::move(archetype)));
}
