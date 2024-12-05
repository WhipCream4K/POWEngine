#include "pch.h"
#include "Archetype.h"

#include "ComponentInfo.h"
#include "Core/Memory/AllocatorContext.h"

#include <numeric>


using namespace powe;

Archetype::Archetype(const Vector<ComponentID>& components) noexcept
{
    const AllocatorContext context{AllocatorScope::Game};
    auto *upStream{context.GetResource()};

    m_ComponentData = Vector<std::byte>(upStream);
    m_ComponentIDs = Vector<ComponentID>(upStream);
    m_EntityToIndex = UnOrderedMap<EntityID, uint32_t>(upStream);

    m_ComponentBlockSize = std::accumulate(components.begin(), components.end(), size_t(0), 
    [](size_t acc, ComponentID id)
    { 
        return acc + ComponentInfo::GetSize(id);
    });
}

Archetype::Archetype(const Set<ComponentID>& components) noexcept
    : Archetype{Vector<ComponentID>{components.begin(), components.end()}}
{
}

const std::byte* Archetype::GetComponentPos(EntityID id) const noexcept
{
    return &m_ComponentData[m_EntityToIndex.at(id) * m_ComponentBlockSize];
}

std::byte* Archetype::GetComponentPos(EntityID id) noexcept
{
    return &m_ComponentData[m_EntityToIndex.at(id) * m_ComponentBlockSize];
}

Set<ComponentID> Archetype::GetComponentIDs() const noexcept
{
    return Set<ComponentID>{m_ComponentIDs.begin(), m_ComponentIDs.end()};
}

bool Archetype::GetComponents(const Set<ComponentID>& query, Vector<CompAddress>& outAddress) noexcept
{
    const Set<ComponentID> compIDs{GetComponentIDs()};

    const bool result{std::includes(query.begin(), query.end(), compIDs.begin(), compIDs.end())};
    if(!result)
        return false;
    
    const Vector<ComponentID> queryVec{query.begin(), query.end()};

    // query all the components in the archetype that match with the query
    for(const auto& [id,idx] : m_EntityToIndex)
    {
        std::byte* src{ GetComponentPos(idx) };

        for(size_t i = 0; i < m_ComponentIDs.size(); ++i)
        {
            const auto compID{ m_ComponentIDs[i] };

            if(std::find(queryVec.begin(), queryVec.end(), compID) == queryVec.end())
            {
                src += ComponentInfo::GetSize(compID);
                continue;
            }

            outAddress.emplace_back(src);
        }

    }

    return true;
}

void Archetype::Remove(EntityID id, ComponentStorage &outComponents, PMRResource *stackAllocator) noexcept
{
    if (auto findItr{m_EntityToIndex.find(id)}; findItr != m_EntityToIndex.end())
    {
        std::byte* src = GetComponentPos(findItr->second);
        
        for(size_t i = 0; i < m_ComponentIDs.size(); ++i)
        {
            const auto compID{ m_ComponentIDs[i] };
            const size_t compSize{ ComponentInfo::GetSize(compID) };

            void* dest{stackAllocator->allocate(compSize)};
            
            const auto moveOp{ComponentInfo::GetMoveOp(compID)};
            moveOp(src, dest);
            src += compSize;

            const SharedPtr<void> managedComponent{dest, ComponentInfo::GetDestroyOp(compID)};
            outComponents.emplace_back(std::make_pair(compID,managedComponent));
        }

        m_EntityToIndex.erase(findItr);

        m_ComponentData.erase(
            m_ComponentData.begin() + (findItr->second * m_ComponentBlockSize), // first pos
            m_ComponentData.begin() + ((findItr->second + 1) * m_ComponentBlockSize)); // last pos
    }
}

void Archetype::Insert(EntityID id, ComponentStorage &&inComponents) noexcept
{
    // sort the block to match the order of the components of this archetype
    if(inComponents.size() != m_ComponentIDs.size())
        return;
    
    SortBlock(inComponents);

    InsertNoSort(id, std::move(inComponents));
}

void Archetype::InsertNoSort(EntityID id, ComponentStorage &&inComponents) noexcept
{
    m_EntityToIndex.try_emplace(id, m_ComponentData.size() / m_ComponentBlockSize);
    m_ComponentData.insert(m_ComponentData.end(), m_ComponentBlockSize, std::byte(0));

    std::byte* dest{&m_ComponentData.back() - m_ComponentBlockSize};

    for(size_t i = 0; i < m_ComponentIDs.size(); ++i)
    {
        const auto compID{ m_ComponentIDs[i] };

        const auto moveOp{ComponentInfo::GetMoveOp(compID)};
        moveOp(inComponents[i].second.get(), dest);
        dest += ComponentInfo::GetSize(compID);
    }
}