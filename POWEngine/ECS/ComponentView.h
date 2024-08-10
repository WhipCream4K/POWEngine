#pragma once

#include "ECSManager.h"

namespace powe
{
    class ComponentView final
    {
    public:
        ComponentView(PMRResource* memResource,
                      ECSManager& manager,
                      const Vector<ComponentID>& compIDs);

        template <ComponentConcept... Args>
        class Range
        {
        public:
            
            Range(ComponentView* refComponents)
                : m_ComponentView{refComponents}
            {
            }

            class Iterator
            {
            public:
                using ArchetypeIterator = UnOrderedMap<IArchetype*, Vector<void*>>::iterator;

                Iterator(const ArchetypeIterator& startArchetype, int index)
                    : m_ArchetypeIterator(startArchetype)
                      , m_IteratorIndex(index)
                {
                }

                Iterator& operator++()
                {
                    m_IteratorIndex += std::tuple_size_v<std::tuple<Args...>>;
                    if (m_IteratorIndex >= m_ArchetypeIterator->second.size())
                    {
                        ++m_ArchetypeIterator;
                        m_IteratorIndex = 0;
                    }
                    return *this;
                }

                Iterator operator++(int)
                {
                    Iterator temp = *this;
                    ++(*this);
                    return temp;
                }

                bool operator==(const Iterator& other) const
                {
                    return m_ArchetypeIterator == other.m_ArchetypeIterator;
                }

                bool operator!=(const Iterator& other) const
                {
                    return !(*this == other);
                }

                std::tuple<std::add_lvalue_reference_t<Args>...> operator*()
                {
                    // return std tie of m_Address in an index sequence of size of Args
                    // m_Iterator + index of Args
                    return TieWithIndex(m_ArchetypeIterator, m_IteratorIndex, std::make_index_sequence<sizeof...(Args)>{});
                }

            private:

                // Helper function to generate the sequence of indices
                template <std::size_t... Is>
                std::tuple<std::add_lvalue_reference_t<Args>...> TieWithIndex(
                    UnOrderedMap<IArchetype*, Vector<void*>>::iterator archetypeIterator,
                    int iteratorIndex,
                    std::index_sequence<Is...>)
                {
                    return std::tie(*static_cast<Args*>(archetypeIterator->second[iteratorIndex + Is])...);
                }
                
                ArchetypeIterator m_ArchetypeIterator;
                int m_IteratorIndex;
            };

            Iterator begin()
            {
                return Iterator(m_ComponentView->m_ComponentsAddresses, 0);
            }

            Iterator end()
            {
                return Iterator(m_ComponentView->m_ComponentsAddresses,
                                m_ComponentView->m_ComponentsAddresses.end(),
                                m_ComponentView->m_ComponentsAddresses.size());
            }

        private:
            ComponentView* m_ComponentView;
        };

    private:
        
        void ResetComponentAddresses(IArchetype* archetype);


        RefWrap<ECSManager> m_ECSManager;
        UnOrderedMap<IArchetype*, Vector<void*>> m_ComponentsAddresses;
    };
}
