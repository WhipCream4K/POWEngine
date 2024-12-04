#pragma once

#include "ECSTypes.h"
#include "ECS/ECSUtils.h"
#include "Archetype.h"

#include "Core/Memory/AllocatorContext.h"

namespace powe
{
	class IArchetype;
	class Entity;
	class ECSManager final
	{
	public:

		// TODO: Finish the run-time manipulation of ECS
		ECSManager();

		bool IsUpdating() const noexcept { return m_IsUpdating; }

		EntityID MakeNewEntityID() { return m_CurrentEntityID++; }
		std::unique_ptr<Entity> CreateEntity() noexcept;

		SharedPtr<IArchetype> GetArchetypeFrom(EntityID entityID) const noexcept;
		SharedPtr<IArchetype> GetArchetypeFrom(const Set<ComponentID>& components) const noexcept;
		void GetArchetypes(const Set<ComponentID>& query,Vector<SharedPtr<IArchetype>>& outArchetypes) const noexcept;
	
		bool Contains(const Set<ComponentID>& query) const noexcept;
		bool Contains(EntityID entityID) const noexcept;

		/// Sumbits a function to be run at the next update loop
		/// Any manipulation of container function should be add by this method or else everything will break
		/// @param action the function to run
		void SumbitLateAction(const std::function<void(ECSManager&)>& action) noexcept;

		template<typename ... Args> requires (ComponentConcept<Args> && ...)
	 	bool HasComponent(EntityID entityID) const noexcept
		{
			const auto archetype{ GetArchetypeFrom(entityID) };
			if(archetype == nullptr)
				return false;
			
			return archetype->HasComponent(MakeComponentVec<Args...>());
		}

		template<typename... Args>
		void AddComponent(EntityID entityID, Args&&... args) noexcept
		{
			if(!IsUpdating())
			{
				// 1. Check if this entity already in an archetype
				const auto archetype{GetArchetypeFrom(entityID)};
				if(archetype == nullptr)
				{
					// 2. If not, create archetype and add component, this is easy
					const auto newArchetype{ CreateArchetype<Args...>() };
					newArchetype->emplace_back(entityID, std::forward_as_tuple(std::forward<Args>(args)...));
					InsertArchetype(std::move(newArchetype));
				}
				else
				{
					// 3. If yes, remove this entity from the archetype
					// 4. Combine the components from old archetype together with the new components
					// 5. Create or get archetype with the combined components and add this entity
					const Set<ComponentID> oldIDs{ archetype->GetComponentIDSet() };
					Set<ComponentID> newIDs{ MakeComponentSet<Args...>() };

					newIDs.insert(oldIDs.begin(), oldIDs.end());
					std::pmr::monotonic_buffer_resource tempResource{};
					
				}
				
				return;				
			}

			// add to await collection. At later stage, all entities will be assigned to the archetype correctly anyway
			auto addComponent = [&awaitCollection = m_AwaitCollection](auto&& arg) {
				
				const AllocatorContext context{ AllocatorScope::Game };
				auto* upStream{ context.GetResource() };
				using ComponentType = decltype(arg);
    		    const ComponentID id{ GetComponentID<ComponentType>() };
				auto memoryBlock{std::allocate_shared<ComponentType>(upStream, std::forward<decltype(arg)>(arg))};
				awaitCollection[id].emplace_back(std::static_pointer_cast<void>(memoryBlock));
    		};

			(addComponent(std::forward<Args>(args)), ...);
		}

		template<typename ... Args> requires (ComponentConcept<Args> && ...)
		void RemoveComponent(EntityID entityID) noexcept
		{
			
		}

		void Remove(EntityID entityID) noexcept;

		template <typename ... Args> requires (ComponentConcept<Args> && ...)
		SharedPtr<Archetype<Args...>> CreateArchetype()
		{
			const Set<ComponentID> componentRange{ MakeComponentSet<Args...>() };
			
			if(Contains(componentRange))
			{
				return GetArchetype(componentRange);
			}

			const AllocatorContext context{ AllocatorScope::Game };

			auto* upStream{ context.GetResource() };

			const SharedPtr<Archetype<Args...>> archetype{
				std::allocate_shared<Archetype<Args...>>(upStream)};


			return archetype;
		}

		

	
	private:

		void InsertArchetype(const SharedPtr<IArchetype>& archetype);

		IndexedMultimap<SharedPtr<IArchetype>> m_Archetypes;
 
		Vector<std::function<void(ECSManager&)>> m_AwaitActions;
		UnOrderedMap<EntityID, ComponentStorage> m_AwaitCollection;

		UnOrderedMap<EntityID, SharedPtr<IArchetype>> m_EntityToArchetype;
		std::atomic<EntityID> m_CurrentEntityID{};
		bool m_IsUpdating{};

	};


}

