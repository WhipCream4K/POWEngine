#pragma once

#include <vector>
#include <variant>
#include <functional>

#include "ComponentIDGen.h"
#include "ECSTypes.h"
#include "Core/CustomTypes.h"

namespace powe
{
    class IArchetype
    {
    public:
        IArchetype() = default;
        IArchetype(const IArchetype&) = default;
        IArchetype& operator=(const IArchetype&) = default;
        IArchetype(IArchetype&&) = default;
        IArchetype& operator=(IArchetype&&) = default;
        virtual ~IArchetype() = default;

        virtual void GetComponents(const Vector<ComponentID>& compIDs, Vector<void*>& outAddress) const = 0;
        virtual void* GetFrontComponentAddress() const = 0;
        virtual void InsertInvalidCallback(std::function<void(IArchetype*)> callback) = 0;
    };

    template <ComponentConcept... Args>
    class Archetype final : public IArchetype
    {
    public:
        using InvalidCallback = std::function<void(IArchetype*)>;

        using ComponentBlock = std::tuple<Args...>;
        using Iterator = typename Vector<ComponentBlock>::iterator;

        Archetype(std::pmr::memory_resource* resource)
            : m_Components(resource)
              , m_EntityToIndex(resource)
              , m_OldCapacity(m_Components.capacity())
        {
        }

        constexpr auto begin() noexcept { return m_Components.begin(); }
        constexpr auto end() noexcept { return m_Components.end(); }

        constexpr auto begin() const noexcept { return m_Components.begin(); }
        constexpr auto end() const noexcept { return m_Components.end(); }

        constexpr auto cbegin() const noexcept { return m_Components.cbegin(); }
        constexpr auto cend() const noexcept { return m_Components.cend(); }

        constexpr auto rbegin() { return m_Components.rbegin(); }
        constexpr auto rend() { return m_Components.rend(); }

        constexpr auto rbegin() const noexcept { return m_Components.rbegin(); }
        constexpr auto rend() const noexcept { return m_Components.rend(); }

        constexpr auto crbegin() const { return m_Components.crbegin(); }
        constexpr auto crend() const { return m_Components.crend(); }

        void GetComponents(const Vector<ComponentID>& compIDs, Vector<void*>& outAddress) const override
        {
            for (const auto& compID : compIDs)
            {
                for (const auto& componentBlock : m_Components)
                {
                    std::apply([&outAddress, &compID]<typename... U>(U const&... components)
                    {
                        if (compID == ComponentIDGen::Get<std::decay_t<U>...>())
                        {
                            outAddress.emplace_back(&components);
                        }
                    }, componentBlock);
                }
            }
        }

        void* GetFrontComponentAddress() const override
        {
            return m_Components.empty() ? nullptr : &m_Components.front();
        }

        std::tuple<ComponentBlock>& operator [](EntityID id) noexcept
        {
            return m_Components[m_EntityToIndex[id]];
        }

        std::tuple<ComponentBlock>& operator [](EntityID id) const noexcept
        {
            return m_Components[m_EntityToIndex.at(id)];
        }

        void emplace_back(EntityID id, Args&&... components)
        {
            m_Components.emplace_back(std::make_tuple<ComponentBlock>(std::move(components)));
            m_EntityToIndex[id] = m_Components.size() - 1;

            // Detect which index of this type is being emplace and store it
            ((m_ComponentToIndex[ComponentIDGen::Get<Args>()] = tuple_index_v<Args, std::tuple<Args...>>), ...);

            if (m_Components.capacity() > m_OldCapacity)
            {
                for (auto& callback : m_InvalidCallbacks)
                {
                    callback(this);
                }
            }
		
        }

        Iterator find(EntityID id)
        {
            if (auto it = m_EntityToIndex.find(id); it != m_EntityToIndex.end())
            {
                return m_Components.begin() + it->second;
            }

            return m_Components.end();
        }

        typename Iterator erase(typename Iterator begin, typename Iterator end)
        {
            for (auto it = m_EntityToIndex.begin(); it != m_EntityToIndex.end();)
            {
                if (it->second >= std::distance(m_Components.begin(), begin) && it->second < std::distance(
                    m_Components.begin(), end))
                {
                    it = m_EntityToIndex.erase(it);
                }
                else
                {
                    ++it;
                }
            }


            return m_Components.erase(begin, end);
        }

        typename Iterator erase(typename Iterator pos)
        {
            for (auto it = m_EntityToIndex.begin(); it != m_EntityToIndex.end();)
            {
                if (it->second == std::distance(m_Components.begin(), pos))
                {
                    it = m_EntityToIndex.erase(it);
                    break;
                }
                ++it;
            }

            return m_Components.erase(pos);
        }

        typename Vector<ComponentBlock>::size_type size() const
        {
            return m_Components.size();
        }

    private:
        Vector<ComponentBlock> m_Components;
        UnOrderedMap<ComponentID, uint32_t> m_ComponentToIndex;
        UnOrderedMap<EntityID, size_t> m_EntityToIndex;
        Vector<InvalidCallback> m_InvalidCallbacks;
        int m_OldCapacity{0};
    };
}
