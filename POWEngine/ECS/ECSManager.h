#pragma once

#include "ECSTypes.h"
#include "Archetype.h"
#include "ECSUtils.h"
#include "Utils/Utils.h"


namespace powe
{
	class IArchetype;
	class ECSManager final
	{
	public:

		ECSManager(PMRResource* memResource);
		
		EntityID CreateEntity() { return m_CurrentEntityID++; }

		template<ComponentConcept... Args>
		EntityID CreateEntity(Args&&... args)
		{
			Archetype<Args...>& archetype{ GetOrCreateArchetype<Args...>() };
			EntityID newID{m_CurrentEntityID++};
			archetype.emplace_back(newID,std::forward<Args>(args)...);
			return newID;
		}

		void GetArchetypes(const Vector<ComponentID>& query,Vector<IArchetype*>& outArchetypes) const;

		
		/**
		 * Return only the first match archetype of the given ids
		 * Use this if want the exact match of the archetype of these ids
		 * @param compIDs Component ID
		 * @return The first match archetype of the given ids
		 */
		IArchetype* GetArchetype(const Vector<ComponentID>& compIDs) const;

		template<typename ...Args> requires (ComponentConcept<Args> && ...)
		IArchetype* GetOrCreateArchetype();

		bool IsContainsArchetype(const Vector<ComponentID>& compIDs) const;
	
	private:

		void InsertArchetype(const Vector<ComponentID>& compIDs,UniquePtr<IArchetype>&& archetype);

		DynamicBitsetRange<UniquePtr<IArchetype>> m_Archetypes;
		UnOrderedMap<EntityID,Vector<ComponentID>> m_EntityArchetypeMap;
		PMRResource* m_MemResource;
		std::atomic<EntityID> m_CurrentEntityID{};

	};

	template <typename ... Args> requires (ComponentConcept<Args> && ...)
	IArchetype* ECSManager::GetOrCreateArchetype()
	{
		const Vector<ComponentID> componentRange{ MakeComponentRange<Args...>() };
		
		if(IsContainsArchetype(componentRange))
		{
			return GetArchetype(componentRange);
		}

		auto archetype{AllocateUnique<Archetype<Args...>>(m_MemResource)};
		InsertArchetype(componentRange,std::move(archetype));
		return archetype.get();
	}
}

