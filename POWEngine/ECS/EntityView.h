#pragma once
#include "Archetype.h"

namespace powe
{
	
	template<ComponentConcept... Args>
	class EntityView final
	{
	public:

		explicit EntityView(Archetype<Args...>& archetype, EntityID id)
			: m_Archetype(archetype)
			, m_EntityID(id)
		{
			const auto findItr = m_Archetype.find(id);
			m_OldAddress = findItr != m_Archetype.end() ? &*findItr : nullptr;
		}

		using ComponentBlock = std::tuple<Args...>;

		// Shouldn't throw
		bool IsValid() const
		{
			return m_OldAddress != nullptr && (m_OldAddress == &m_Archetype[m_EntityID]);
		}

		operator bool() const
		{
			return IsValid();
		}

		ComponentBlock* Get()
		{
			if (IsValid())
			{
				return m_OldAddress;
			}

			auto findItr{ m_Archetype.find(m_EntityID) };
			if (findItr != m_Archetype.end())
			{
				m_OldAddress = &*findItr;
				return m_OldAddress;
			}

			return nullptr;

		}

	private:

		Archetype<Args...>& m_Archetype;
		ComponentBlock* m_OldAddress;
		EntityID m_EntityID;
	};
}
