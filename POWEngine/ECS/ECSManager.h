#pragma once

#include "ECSTypes.h"
#include "ECS/ECSUtils.h"
#include "Archetype.h"


namespace powe
{
	class IArchetype;
	class Entity;
	class ECSManager final
	{
	public:

		ECSManager() = default;
		ECSManager(const SharedPtr<PMRResource>& upStream);

		EntityID MakeNewEntityID() { return m_CurrentEntityID++; }
		Entity CreateEntity() noexcept;

		SharedPtr<IArchetype> GetArchetypeFrom(EntityID entityID) const noexcept;
		
		// Add New Archetype or Add component to existing archetype
		void ScheduleAdd(const SharedPtr<IArchetype>& newArchetype) noexcept;
		void ScheduleAdd(EntityID entityID,const Vector<std::pair<ComponentID,SharedPtr<void>>>&) noexcept;

		template<typename... Args>
		void ScheduleAdd(EntityID entityID,Args&&... args) noexcept
		{
			
		}

		void GetArchetypes(const Vector<ComponentID>& query,Vector<IArchetype*>& outArchetypes) const;
		
		/**
		 * Return only the first match archetype of the given ids
		 * Use this if want the exact match of the archetype of these ids
		 * @param compIDs Component ID
		 * @return The first match archetype of the given ids
		 */
		IArchetype* GetArchetype(const Vector<ComponentID>& query) const noexcept;
		IArchetype* GetArchetype(const Set<ComponentID>& query) const noexcept;

		bool ContainsArchetype(const Vector<ComponentID>& query) const noexcept;
		bool ContainsArchetype(const Set<ComponentID>& query) const noexcept;

		template<typename ... Args> requires (ComponentConcept<Args> && ...)
	 	bool HasComponent(EntityID entityID) const noexcept
		{
			const auto archetype{ GetArchetypeFrom(entityID) };
			if(archetype == nullptr)
				return false;
			
			return archetype->HasComponent(MakeComponentVec<Args...>());
		}

		template <typename ... Args> requires (ComponentConcept<Args> && ...)
		SharedPtr<Archetype<Args...>> CreateArchetype()
		{
			const Set<ComponentID> componentRange{ MakeComponentSet<Args...>() };
			
			if(ContainsArchetype(componentRange))
			{
				return GetArchetype(componentRange);
			}
			const SharedPtr<Archetype<Args...>> archetype{std::allocate_shared<Archetype<Args...>>(m_UpStream,m_UpStream)};
			return archetype;
		}
	
	private:

		void InsertArchetype(const Set<ComponentID>& compIDs,const SharedPtr<IArchetype> archetype);

		SharedPtr<PMRResource> m_UpStream;

		IndexedMultimap<SharedPtr<IArchetype>> m_Archetypes;
		Vector<SharedPtr<IArchetype>> m_ToBeAddedArchetypes;


		UnOrderedMap<EntityID, SharedPtr<IArchetype>> m_EntityToArchetype;
		std::atomic<EntityID> m_CurrentEntityID{};

	};


}

