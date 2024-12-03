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

		ECSManager(const SharedPtr<PMRResource>& memResource);

		EntityID MakeNewEntityID() { return m_CurrentEntityID++; }
		Entity CreateEntity() noexcept;
		
		// EntityID CreateEntity() { return m_CurrentEntityID++; }

		// template<typename... Args> requires (ComponentConcept<Args> && ...)
		// EntityID CreateEntity(Args&&... args)
		// {
		// 	Archetype<Args...>& archetype{ GetOrCreateArchetype<Args...>() };
		// 	EntityID newID{m_CurrentEntityID++};
		// 	archetype.emplace_back(newID,std::forward<Args>(args)...);
		// 	return newID;
		// }

		void GetArchetypes(const Vector<ComponentID>& query,Vector<IArchetype*>& outArchetypes) const;
		
		/**
		 * Return only the first match archetype of the given ids
		 * Use this if want the exact match of the archetype of these ids
		 * @param compIDs Component ID
		 * @return The first match archetype of the given ids
		 */
		IArchetype* GetArchetype(const Vector<ComponentID>& query) const noexcept;
		IArchetype* GetArchetype(const Set<ComponentID>& query) const noexcept;

		SharedPtr<PMRResource> GetResource() const noexcept { return m_MemResource; }

		bool ContainsArchetype(const Vector<ComponentID>& query) const noexcept;
		bool ContainsArchetype(const Set<ComponentID>& query) const noexcept;

		template <typename ... Args> requires (ComponentConcept<Args> && ...)
		SharedPtr<Archetype<Args...>> GetOrCreateArchetype()
		{
			const Set<ComponentID> componentRange{ MakeComponentSet<Args...>() };
			
			if(ContainsArchetype(componentRange))
			{
				return GetArchetype(componentRange);
			}
	
			const SharedPtr<Archetype<Args...>> archetype{std::allocate_shared<Archetype<Args...>>(m_MemResource,m_MemResource)};
			InsertArchetype(componentRange,archetype);
			return archetype;
		}
	
	private:

		void InsertArchetype(const Set<ComponentID>& compIDs,const SharedPtr<IArchetype> archetype);
		// void InsertArchetype(const Vector<ComponentID>& compIDs,UniquePtr<IArchetype>&& archetype);

		IndexedMultimap<SharedPtr<IArchetype>> m_Archetypes;
		SharedPtr<PMRResource> m_MemResource;
		std::atomic<EntityID> m_CurrentEntityID{};

	};


}

