#include "pch.h"

#include "ECSManager.h"
#include "Entity.h"

using namespace powe;

powe::ECSManager::ECSManager()
{
    const AllocatorContext context{};
    auto *upStream{context.GetResource()};

    m_Archetypes = ComponentSetMultimap<SharedPtr<Archetype>>(upStream);
    m_AwaitActions = Vector<std::function<void(ECSManager &)>>{upStream};
    m_AwaitEntitiesCollection = UnOrderedMap<EntityID, ComponentStorage>(upStream);
}

std::unique_ptr<Entity> ECSManager::CreateEntity() noexcept
{
    return std::make_unique<Entity>(*this);
}

UniquePtr<Entity> ECSManager::CreateEntity(std::pmr::memory_resource *allocator) noexcept
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
    auto removeFunc = [entityID](ECSManager &manager) {
        const auto archetype(manager.GetArchetypeFrom(entityID));
        archetype->Remove(entityID);
    };

    if (!IsUpdating())
    {
        removeFunc(*this);
    }
    else
    {
        if (auto findItr{m_AwaitEntitiesCollection.find(entityID)}; findItr != m_AwaitEntitiesCollection.end())
        {
            m_AwaitEntitiesCollection.erase(findItr);
        }
        else
        {

            m_AwaitActions.emplace_back(std::move(removeFunc));
        }
    }

    m_EntityToArchetype.erase(entityID);
}

void ECSManager::GetArchetypes(const Set<ComponentID> &query,
                               Vector<SharedPtr<Archetype>> &outArchetypes) const noexcept
{
    for (const auto &[archetypeKey, archetypes] : m_Archetypes)
    {
        if (archetypes->HasComponents(query))
        {
            outArchetypes.emplace_back(archetypes);
        }
    }
}

void ECSManager::GetArchetypes(const Vector<ComponentID> &query,
                               Vector<SharedPtr<Archetype>> &outArchetypes) const noexcept
{
    GetArchetypes(Set<ComponentID>{query.begin(), query.end()}, outArchetypes);
}

bool ECSManager::Contains(const Set<ComponentID> &query) const noexcept
{
    return GetArchetypeFrom(query) != nullptr;
}

bool ECSManager::Contains(const Vector<ComponentID> &query) const noexcept
{
    return GetArchetypeFrom(query) != nullptr;
}

bool ECSManager::Contains(EntityID entityID) const noexcept
{
    return GetArchetypeFrom(entityID) != nullptr;
}

void ECSManager::CalculateMemoryForResolve(EntityID id,const Vector<ComponentID>& newComponents) noexcept
{
    // Pre-register the memory needed during resolve
    const auto archetype{GetArchetypeFrom(id)};
    if (archetype)
    {
        Vector<ComponentID> combinedIDs{archetype->GetComponentVec()};
        combinedIDs.insert(combinedIDs.end(), newComponents.begin(), newComponents.end());

        const size_t requiredSize =
            std::accumulate(combinedIDs.begin(), combinedIDs.end(), size_t(0),
                            [](std::size_t sum, const ComponentID &id) { return sum + ComponentInfo::GetSize(id); });

        m_EstimatedMemDuringResolve += requiredSize;
        
        return;
    }

    const size_t requiredSize = std::accumulate(newComponents.begin(), newComponents.end(), size_t(0),
        [](std::size_t sum, const ComponentID &id) { return sum + ComponentInfo::GetSize(id); });

    m_EstimatedMemDuringResolve += requiredSize;
    
}

void ECSManager::ResolveEntities() noexcept
{
    if(m_AwaitEntitiesCollection.empty() && m_AwaitActions.empty())
        return;

    {
        std::pmr::monotonic_buffer_resource tempResource{m_EstimatedMemDuringResolve};

        for (auto &[entityID, components] : m_AwaitEntitiesCollection)
        {
            if (const auto archetype{GetArchetypeFrom(entityID)}; archetype)
            {
                // pull the components out if this entity has a component
                ComponentStorage outComponents{};
                archetype->Remove(entityID, outComponents, &tempResource);

                components.insert(components.end(), outComponents.begin(), outComponents.end());
            }

            Vector<ComponentID> compIDs{};
            for(auto& [componentID,component] : components)
            {
                compIDs.emplace_back(componentID);
            }

            const auto archetype{CreateArchetype(compIDs)};
            archetype->Insert(entityID, std::move(components));
        }
    }

    for(auto& action : m_AwaitActions)
    {
        action(*this); 
    }

    m_AwaitActions.clear();
    m_AwaitEntitiesCollection.clear();

    m_EstimatedMemDuringResolve = 0;
}