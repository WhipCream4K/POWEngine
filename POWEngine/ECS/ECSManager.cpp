#include "pch.h"

#include "ECSUtils.h"
#include "ECSManager.h"
#include "Entity.h"

using namespace powe;

powe::ECSManager::ECSManager(const SharedPtr<PMRResource>& memResource)
	: m_Archetypes{ memResource.get() }
	, m_MemResource(memResource)
{
}

Entity ECSManager::CreateEntity() noexcept
{
	return Entity{ *this };
}

void powe::ECSManager::GetArchetypes(const Vector<ComponentID>& query, Vector<IArchetype*>& outArchetypes) const
{
	const Set<ComponentID> querySet{ query.begin(), query.end() };

	for(const auto& [archetypeKey, archetypes] : m_Archetypes)
	{
		if(IsArchetypeMatch(archetypeKey, querySet))
		{
			outArchetypes.emplace_back(archetypes.get());
		}
	}
}

powe::IArchetype* powe::ECSManager::GetArchetype(const Vector<ComponentID>& query) const noexcept
{
	const Set<ComponentID> queryKey{ query.begin(), query.end() };

	for(const auto& [archetypeKey, archetypes] : m_Archetypes)
	{
		if(IsArchetypeMatch(archetypeKey, queryKey))
		{
			return archetypes.get();
		}
	}

	return nullptr;
}

IArchetype* ECSManager::GetArchetype(const Set<ComponentID>& query) const noexcept
{
	for(const auto& [archetypeKey, archetypes] : m_Archetypes)
	{
		if(IsArchetypeMatch(archetypeKey, query))
		{
			return archetypes.get();
		}
	}

	return nullptr;
}

bool powe::ECSManager::ContainsArchetype(const Vector<ComponentID>& query) const noexcept
{
	const Set<ComponentID> queryKey{ query.begin(), query.end() };

	for(const auto& [archetypeKey, archetypes] : m_Archetypes)
	{
		if(IsArchetypeMatch(archetypeKey, queryKey))
		{
			return true;
		}
	}

	return false;
}

bool ECSManager::ContainsArchetype(const Set<ComponentID>& query) const noexcept
{
	for(const auto& [archetypeKey, archetypes] : m_Archetypes)
	{
		if(IsArchetypeMatch(archetypeKey, query))
		{
			return true;
		}
	}

	return false;
}