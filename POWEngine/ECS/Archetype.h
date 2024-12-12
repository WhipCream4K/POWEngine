#pragma once

#include <vector>
#include <functional>

#include "ComponentInfo.h"
#include "ECSTypes.h"
#include "Core/CustomTypes.h"
#include "Core/Memory/AllocatorContext.h"

#include <algorithm>
#include <ranges>

namespace powe
{
    class Archetype final
    {
        using InvalidCallback = std::function<void(Archetype&)>;

    public:
    
        Archetype(const Vector<ComponentID>& components) noexcept;
        Archetype(const Set<ComponentID>& components) noexcept;
        Archetype(const Archetype&) = default;
        Archetype& operator=(const Archetype&) = default;
        Archetype(Archetype&&) = default;
        Archetype& operator=(Archetype&&) = default;
        ~Archetype();

        bool GetComponents(const Set<ComponentID>& query, Vector<CompAddress>& outAddress) noexcept;
        bool GetComponents(const Vector<ComponentID>& query, Vector<CompAddress>& outAddress) noexcept;
        Set<ComponentID> GetComponentIDs() const noexcept;
        constexpr const Vector<ComponentID>& GetComponentVec() const noexcept { return m_ComponentIDs; }

        // Remove entity from archetype and return the tuple of removed components
        void Remove(EntityID id, ComponentStorage& outComponents,PMRResource* stackAllocator) noexcept;
        void Remove(EntityID id) noexcept;
        void Insert(EntityID id, ComponentStorage&& inComponents) noexcept;
        void InsertNoSort(EntityID id, ComponentStorage&& inComponents) noexcept;
        
        bool HasComponents(const Set<ComponentID>& query) const noexcept;

        void CallInvalidCallees() noexcept;
        [[nodiscard]] size_t RegisterInvalidCallee(InvalidCallback callee) noexcept;
        void RemoveInvalidCallee(size_t id) noexcept;

        template<ComponentConcept T>
        constexpr bool HasComponent() const noexcept
        {
            return std::ranges::find(m_ComponentIDs, ComponentInfo::GetID<T>()) != m_ComponentIDs.end();
        }

        // return size of elements in archetype
        size_t size() const noexcept
        {
            return m_EntityToIndex.size();
        }
        
        bool contains(EntityID id) const noexcept
        {
            return m_EntityToIndex.contains(id);
        }

        constexpr bool IsValid() const noexcept
        {
            return !m_ToBeRemoved;
        }

        constexpr bool empty() const noexcept
        {
            return m_ComponentIDs.empty();
        }


    private:

        void SortBlock(ComponentStorage& blockComponents) const noexcept;

        const std::byte* GetComponentPos(EntityID id) const noexcept;
        std::byte* GetComponentPos(EntityID id) noexcept;

        // Block of component data like [1,2,3],[1,2,3]
        Vector<std::byte> m_ComponentData;

        size_t m_ComponentBlockSize;
        Vector<ComponentID> m_ComponentIDs;
        UnOrderedMap<EntityID, size_t> m_EntityToIndex;
        UnOrderedMap<size_t,InvalidCallback> m_InvalidCallbacks;
        bool m_ToBeRemoved{};

    };
}
