#include "pch.h"

#include "ECSManager.h"
#include "ECSUtils.h"
#include "Entity.h"

using namespace powe;

powe::ECSManager::ECSManager()
{
    const AllocatorContext context{};
    auto *upStream{context.GetResource()};

    m_Archetypes = IndexedMultimap<SharedPtr<Archetype>>(upStream);
    m_AwaitActions = Vector<std::function<void(ECSManager&)>>{upStream};
    m_AwaitEntitiesCollection = UnOrderedMap<EntityID, ComponentStorage>(upStream);
}

std::unique_ptr<Entity> ECSManager::CreateEntity() noexcept
{
    return std::make_unique<Entity>(*this);
}

UniquePtr<Entity> ECSManager::CreateEntity(std::pmr::memory_resource* allocator) noexcept
{
    return AllocateUnique<Entity>(*this, allocator);
}

SharedPtr<Archetype> ECSManager::GetArchetypeFrom(EntityID entityID) const noexcept
{
    for (const auto &[archetypeKey, archetypes] : m_Archetypes)
    {
        if (archetypes->contains(entityID))
        {
            return archetypes;
        }
    }

    return nullptr;
}

SharedPtr<Archetype> ECSManager::GetArchetypeFrom(const Set<ComponentID> &components) const noexcept
{
    for (const auto &[archetypeKey, archetypes] : m_Archetypes)
    {
        if (archetypes->HasComponents(components))
        {
            return archetypes;
        }
    }

    return nullptr;
}

SharedPtr<Archetype> ECSManager::GetArchetypeFrom(const Vector<ComponentID> &components) const noexcept
{
    Set<ComponentID> compSet{components.begin(), components.end()};
    return GetArchetypeFrom(compSet);
}

void ECSManager::Remove(EntityID entityID) noexcept
{
	auto removeFunc = [entityID](ECSManager& manager) {

		const auto archetype(manager.GetArchetypeFrom(entityID));
		archetype->Remove(entityID);
	};

	if(!IsUpdating())
	{
		removeFunc(*this);
	}
	else 
	{
		if(auto findItr{m_AwaitEntitiesCollection.find(entityID)}; 
			findItr != m_AwaitEntitiesCollection.end())
		{
			m_AwaitEntitiesCollection.erase(findItr);
		}
		else {

			m_AwaitActions.emplace_back(std::move(removeFunc));
		}
	}


	m_EntityToArchetype.erase(entityID);
}

void ECSManager::GetArchetypes(const Set<ComponentID>& query, Vector<SharedPtr<Archetype>>& outArchetypes) const noexcept
{
    for (const auto &[archetypeKey, archetypes] : m_Archetypes)
    {
        if (archetypes->HasComponents(query))
        {
            outArchetypes.emplace_back(archetypes);
        }
    }
}

bool ECSManager::Contains(const Set<ComponentID>& query) const noexcept
{
    return GetArchetypeFrom(query) != nullptr;
}

bool ECSManager::Contains(const Vector<ComponentID>& query) const noexcept
{
    return GetArchetypeFrom(query) != nullptr;
}

bool ECSManager::Contains(EntityID entityID) const noexcept
{
    return GetArchetypeFrom(entityID) != nullptr;
}