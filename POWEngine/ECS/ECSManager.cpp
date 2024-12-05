#include "pch.h"

#include "ECSUtils.h"
#include "ECSManager.h"
#include "Entity.h"


using namespace powe;

powe::ECSManager::ECSManager()
{
	const AllocatorContext context{ AllocatorScope::Game };
	auto* upStream{ context.GetResource() };
	
}

std::unique_ptr<Entity> ECSManager::CreateEntity() noexcept
{
	return std::make_unique<Entity>(*this);
}


SharedPtr<Archetype> ECSManager::GetArchetypeFrom(EntityID entityID) const noexcept
{
	for(const auto& [archetypeKey, archetypes] : m_Archetypes)
	{
		if(archetypes->contains(entityID))
		{
			return archetypes;
		}
	}

	return nullptr;
}

SharedPtr<Archetype> ECSManager::GetArchetypeFrom(const Set<ComponentID>& components) const noexcept
{
	for(const auto& [archetypeKey, archetypes] : m_Archetypes)
	{
		if(archetypes->HasComponent(components))
		{
			return archetypes;
		}
	}

	return nullptr;
}

SharedPtr<Archetype> ECSManager::GetArchetypeFrom(const Vector<ComponentID>& components) const noexcept
{
	Set<ComponentID> compSet{ components.begin(), components.end() };
	return GetArchetypeFrom(compSet);
}



// void powe::ECSManager::GetArchetypes(const Vector<ComponentID>& query, Vector<Archetype*>& outArchetypes) const
// {
// 	const Set<ComponentID> querySet{ query.begin(), query.end() };

// 	for(const auto& [archetypeKey, archetypes] : m_Archetypes)
// 	{
// 		if(IsArchetypeMatch(archetypeKey, querySet))
// 		{
// 			outArchetypes.emplace_back(archetypes.get());
// 		}
// 	}
// }

// powe::Archetype* powe::ECSManager::GetArchetype(const Vector<ComponentID>& query) const noexcept
// {
// 	const Set<ComponentID> queryKey{ query.begin(), query.end() };

// 	for(const auto& [archetypeKey, archetypes] : m_Archetypes)
// 	{
// 		if(IsArchetypeMatch(archetypeKey, queryKey))
// 		{
// 			return archetypes.get();
// 		}
// 	}

// 	return nullptr;
// }

// Archetype* ECSManager::GetArchetype(const Set<ComponentID>& query) const noexcept
// {
// 	for(const auto& [archetypeKey, archetypes] : m_Archetypes)
// 	{
// 		if(IsArchetypeMatch(archetypeKey, query))
// 		{
// 			return archetypes.get();
// 		}
// 	}

// 	return nullptr;
// }

// bool powe::ECSManager::ContainsArchetype(const Vector<ComponentID>& query) const noexcept
// {
// 	const Set<ComponentID> queryKey{ query.begin(), query.end() };

// 	for(const auto& [archetypeKey, archetypes] : m_Archetypes)
// 	{
// 		if(IsArchetypeMatch(archetypeKey, queryKey))
// 		{
// 			return true;
// 		}
// 	}

// 	return false;
// }

// bool ECSManager::ContainsArchetype(const Set<ComponentID>& query) const noexcept
// {
// 	for(const auto& [archetypeKey, archetypes] : m_Archetypes)
// 	{
// 		if(IsArchetypeMatch(archetypeKey, query))
// 		{
// 			return true;
// 		}
// 	}

// 	return false;
// }