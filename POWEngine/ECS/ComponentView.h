#pragma once

#include "ECSManager.h"

namespace powe
{
    class ComponentView final
    {
    public:

        using ArchetypeComponentHandle = UnOrderedMap<Archetype*, Vector<CompAddress>>;

        ComponentView(ECSManager& manager,
                      const Vector<ComponentID>& compIDs);

        ArchetypeComponentHandle& GetComponentsAddresses() { return m_ComponentsAddresses; }
        const Vector<ComponentID>& GetComponentIDs() const { return m_ComponentIDs; }

        template<typename... Args>
        class Iterator;

        template<typename... Args>
        class Iterator<std::tuple<Args...>>
        {
        public:

            Iterator(ArchetypeComponentHandle::iterator componentView)
                : m_ArchetypeIterator{componentView}
            {
            }

            Iterator& operator++()
            {
                ++m_ArchetypeIterator;
                return *this;
            }

            Iterator operator++(int)
            {
                Iterator temp{ *this };
                ++m_ArchetypeIterator;
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
                return TieWithIndex(m_ArchetypeIterator, std::make_index_sequence<sizeof...(Args)>{});
            }

        private:

            // Helper function to generate the sequence of indices
            template <std::size_t... Is>
            std::tuple<std::add_lvalue_reference_t<Args>...> TieWithIndex(
                ArchetypeComponentHandle::iterator archetypeIterator,
                std::index_sequence<Is...>)
            {
                // return std::tie(*static_cast<Args*>(archetypeIterator->second[iteratorIndex + Is])...);
                return std::tie(*static_cast<Args*>(archetypeIterator->second[Is])...);
            }

            ArchetypeComponentHandle::iterator m_ArchetypeIterator;
        };


        template<typename Visitor>
        constexpr void Visit(Visitor&& visitor)
        {
            using FI = FuncInfo<Visitor>;
            using TupleArgsTypes = FI::arg_types;
            for(auto it = begin<TupleArgsTypes>(); it != end<TupleArgsTypes>(); ++it)
            {
                std::apply(visitor, *it);
            }
        }

        constexpr bool empty() const { return m_ComponentsAddresses.empty(); }

        template<typename... Args>
        Iterator<Args...> begin()
        {
            return Iterator<Args...>(m_ComponentsAddresses.begin());
        }

        template<typename... Args>
        Iterator<Args...> end()
        {
            return Iterator<Args...>(m_ComponentsAddresses.end());
        }



    private:

        void ResetComponentAddresses(Archetype* archetype);

        Vector<ComponentID> m_ComponentIDs;
        ArchetypeComponentHandle m_ComponentsAddresses;
    };
}
