#pragma once

#include <vector>

#include "ECSTypes.h"
#include "Core/CustomTypes.h"

namespace powe
{
	template<typename T>
	concept ComponentConcept = std::is_copy_constructible_v<T> && !std::is_pointer_v<T> && std::is_standard_layout_v<T>;

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

		void emplace_back(EntityID id, Args&&... components)
		{
			m_Components.emplace_back(std::make_tuple<ComponentBlock>(std::move(components));
			m_EntityToIndex.emplace_back(id);
		}

		Iterator find(EntityID id)
		{
			const auto findItr{ std::find(m_EntityToIndex.begin(), m_EntityToIndex.end(), id) };
			if (findItr != m_EntityToIndex.end())
			{
				return m_Components.begin() + std::distance(m_EntityToIndex.begin(), findItr);
			}

			return m_Components.end();
		}

		typename Iterator erase(typename Iterator begin, typename Iterator end)
		{
			m_EntityToIndex.erase(m_EntityToIndex.begin() + std::distance(m_Components.begin(), begin),
				m_EntityToIndex.begin() + std::distance(m_Components.begin(), end));

			return m_Components.erase(begin, end);
		}

		typename Iterator erase(typename Iterator pos)
		{
			m_EntityToIndex.erase(m_EntityToIndex.begin() + std::distance(m_Components.begin(), pos));
			return m_Components.erase(pos);
		}

		typename Vector<ComponentBlock>::size_type size() const
		{
			return m_Components.size();
		}

	private:

		Vector<ComponentBlock> m_Components;

		// This will served as EntityID to index mapping.
		// Where the index of the vector will be the index of the component block
		// and the EntityID will be the entity that owns the component block
		Vector<EntityID> m_EntityToIndex;
	};

}


