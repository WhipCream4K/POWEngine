#pragma once

#include <vector>

#include "ECSTypes.h"
#include "Core/CustomTypes.h"

namespace powe
{

	template<ComponentConcept... Args>
	class Archetype final
	{
	public:

		using ComponentBlock = std::tuple<Args...>;
		using Iterator = typename Vector<ComponentBlock>::iterator;

		Archetype() = default;

		Archetype(std::pmr::memory_resource* resource)
			: m_Components(resource)
			, m_EntityToIndex(resource)
		{
		}

		constexpr auto begin() noexcept { return m_Components.begin(); }
		constexpr auto end()  noexcept { return m_Components.end(); }

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

		std::tuple<ComponentBlock>& operator [](EntityID id) noexcept
		{
			return m_Components[m_EntityToIndex[id]];
		}

		std::tuple<ComponentBlock>& operator [](EntityID id) const noexcept
		{
			return m_Components[m_EntityToIndex[id]];
		}

		void emplace_back(EntityID id, Args&&... components)
		{
			m_Components.emplace_back(std::make_tuple<ComponentBlock>(std::move(components)));
			m_EntityToIndex[id] = m_Components.size() - 1;
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
			for (auto it = m_EntityToIndex.begin() ; it != m_EntityToIndex.end();)
			{
				if (it->second >= std::distance(m_Components.begin(), begin) && it->second < std::distance(m_Components.begin(), end))
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

			for (auto it = m_EntityToIndex.begin() ; it != m_EntityToIndex.end();)
			{
				if (it->second == std::distance(m_Components.begin(), pos))
				{
					it = m_EntityToIndex.erase(it);
					break;
				}
				else
				{
					++it;
				}
			}

			return m_Components.erase(pos);
		}

		typename Vector<ComponentBlock>::size_type size() const
		{
			return m_Components.size();
		}

	private:

		Vector<ComponentBlock> m_Components;
		UnOrderedMap<EntityID,size_t> m_EntityToIndex;
	};

	template<ComponentConcept... Args>
	class ArchetypeView
	{
	public:
	};

}


