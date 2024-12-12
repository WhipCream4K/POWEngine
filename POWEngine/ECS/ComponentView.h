#pragma once

#include "ECSManager.h"

namespace powe
{
    class ComponentView final
    {
        using IndexAndSize = std::pair<size_t, size_t>;

        struct ArchetypeViewHandle
        {
            Archetype* archetype;
            size_t index;
            size_t size;
        };

    public:
    
        ComponentView(ECSManager& manager,
                      const Vector<ComponentID>& compIDs);

        ~ComponentView();

        const Vector<ComponentID>& GetComponentIDs() const noexcept { return m_ComponentIDs; }

        template<typename... Args>
        class Iterator
        {
        public:

            Iterator(ComponentView& cv,size_t archetypeIndex)
                : m_ComponentView{cv}
                , m_ArchetypeIndex(archetypeIndex)
                , m_EntityIndex()
            {
            }

            Iterator& operator++()
            {
                // increment the entity index first and check if it is out of range from ArchetypeViews or not
                // if it is out of range, increment the archetype index and set the entity index to 0
                ++m_EntityIndex;
                if(m_EntityIndex >= m_ComponentView.m_ArchetypeViews[m_ArchetypeIndex].size)
                {
                    ++m_ArchetypeIndex;
                    m_EntityIndex = 0;
                }
                return *this;
            }

            Iterator operator++(int)
            {
                Iterator temp{ *this };
                ++(*this);
                return temp;
            }

            bool operator==(const Iterator& other) const
            {
                return m_ArchetypeIndex == other.m_ArchetypeIndex && m_EntityIndex == other.m_EntityIndex;
            }

            bool operator!=(const Iterator& other) const
            {
                return !(*this == other);
            }

            std::tuple<std::add_lvalue_reference_t<Args>...> operator*()
            {
                // return std tie of m_Address in an index sequence of size of Args
                // m_Iterator + index of Args
                return TieWithIndex(GetCurrentAddress(), std::make_index_sequence<sizeof...(Args)>{});
            }

        private:

            Vector<CompAddress>::iterator GetCurrentAddress() noexcept
            {
                return m_ComponentView.m_ComponentsAddresses.begin() + 
                m_ComponentView.m_ArchetypeViews[m_ArchetypeIndex].index + 
                m_EntityIndex;
            }

            // Helper function to generate the sequence of indices
            template <std::size_t... Is>
            std::tuple<std::add_lvalue_reference_t<Args>...> TieWithIndex(
                Vector<CompAddress>::iterator archetypeIterator,
                std::index_sequence<Is...>)
            {
                return std::tie(*static_cast<Args*>(archetypeIterator[Is])...);
            }

            ComponentView& m_ComponentView;
            size_t m_ArchetypeIndex{};
            size_t m_EntityIndex{};
        };

        bool empty() const noexcept
        { 
            return m_ComponentsAddresses.empty();
        }

        template<typename... Args>
        auto View()
        {
            struct Iterable
            {
                ComponentView& cv;
                auto begin() { return Iterator<Args...>(cv, 0); }
                auto end() { return Iterator<Args...>(cv, cv.m_ArchetypeViews.size()); }
            };

            return Iterable{ *this };
        }



    private:

        Vector<ComponentID> m_ComponentIDs;
        Vector<CompAddress> m_ComponentsAddresses;
        Vector<ArchetypeViewHandle> m_ArchetypeViews;
        size_t m_InvalidCalleeID{ 0 };
    };
}
