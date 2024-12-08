#pragma once

#include <numeric>

#include "ECSTypes.h"
#include "ECS/ECSUtils.h"
#include "Archetype.h"
#include "Core/Memory/AllocatorContext.h"


namespace powe
{
	class Archetype;
	class Entity;
	class ECSManager final
	{
	public:

		// TODO: Finish the run-time manipulation of ECS
		ECSManager();

		bool IsUpdating() const noexcept { return m_IsUpdating; }

		EntityID MakeNewEntityID() { return m_CurrentEntityID++; }
		std::unique_ptr<Entity> CreateEntity() noexcept;
		UniquePtr<Entity> CreateEntity(std::pmr::memory_resource* allocator) noexcept;

		SharedPtr<Archetype> GetArchetypeFrom(EntityID entityID) const noexcept;
		SharedPtr<Archetype> GetArchetypeFrom(const Set<ComponentID>& components) const noexcept;
		SharedPtr<Archetype> GetArchetypeFrom(const Vector<ComponentID>& components) const noexcept;
		void GetArchetypes(const Set<ComponentID>& query,Vector<SharedPtr<Archetype>>& outArchetypes) const noexcept;
	
		bool Contains(const Set<ComponentID>& query) const noexcept;
		bool Contains(const Vector<ComponentID>& query) const noexcept;
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
			
			return archetype->HasComponents(MakeComponentSet<Args...>());
		}

		void Step(float deltaTime);

		template<typename... Args>
		void AddComponents(EntityID entityID, Args&&... args) noexcept
		{
			if(!IsUpdating())
			{
				// 1. Check if this entity already in an archetype
				auto archetype{GetArchetypeFrom(entityID)};
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
					// 5. Create or get the archetype,add the combined components to it
					const Set<ComponentID> oldIDs{ archetype->GetComponentIDs() };
					Set<ComponentID> newIDs{ MakeComponentSet<Args...>() };

					newIDs.insert(oldIDs.begin(), oldIDs.end());

					// Calculate the required size
					const size_t requiredSize = std::accumulate(newIDs.begin(), newIDs.end(), size_t(0),
					    [](std::size_t sum, const ComponentID& id) {
					        return sum + ComponentInfo::GetSize(id);
					    });

					// Create the temporary resource
					std::pmr::monotonic_buffer_resource tempResource(requiredSize);

					ComponentStorage combinedComponents(newIDs.size());
					archetype->Remove(entityID, combinedComponents, &tempResource);

					// Combine the components
					auto addComponents = [&combinedComponents,&tempResource](auto&& arg) {
						using ComponentType = decltype(arg);
						SharedPtr<ComponentType> managedMemory{ std::allocate_shared<ComponentType>(tempResource, 
						std::forward<decltype(arg)>(arg)) };

						combinedComponents.emplace_back(std::make_pair(
							ComponentInfo::GetID<ComponentType>(), 
							std::static_pointer_cast<void>(managedMemory)));
					};

					(addComponents(std::forward<Args>(args)), ...);

					// Create or get the archetype,add the combined components to it
					archetype = CreateArchetype(Vector<ComponentID>(newIDs.begin(), newIDs.end()));
					archetype->InsertNoSort(entityID, std::move(combinedComponents));

				}
				
				return;				
			}

			// add to await collection. At later stage, all entities will be assigned to the archetype correctly anyway
			auto addComponent = [&awaitCollection = m_AwaitEntitiesCollection,&entityID](auto&& arg) {
				
				const AllocatorContext context{ AllocatorScope::Game };
				auto* upStream{ context.GetResource() };
				using ComponentType = decltype(arg);
    		    const ComponentID id{ ComponentInfo::GetID<ComponentType>() };
				auto memoryBlock{std::allocate_shared<ComponentType>(upStream, std::forward<decltype(arg)>(arg))};
				awaitCollection[entityID].emplace_back(std::make_pair(id, std::static_pointer_cast<void>(memoryBlock)));
    		};

			(addComponent(std::forward<Args>(args)), ...);
		}

		template<typename... Args> requires (ComponentConcept<Args> && ...)
		void RemoveComponents(EntityID entityID) noexcept
		{
			auto removeFunc = [entityID](ECSManager& manager) {
				
				auto archetype{manager.GetArchetypeFrom(entityID)};
				if(archetype == nullptr)
					return;

				Set<ComponentID> compIDs{ MakeComponentSet<Args...>() };

				const size_t requiredSize{std::accumulate(compIDs.begin(), compIDs.end(), size_t(0),
					[](std::size_t sum, const ComponentID& id) {
						return sum + ComponentInfo::GetSize(id);
				})};
				
				std::pmr::monotonic_buffer_resource tempResource(requiredSize);
				
				ComponentStorage outComponents{};
				archetype->Remove(entityID,outComponents,&tempResource);

				// remove the old component
				for(auto itr = outComponents.begin(); itr != outComponents.end(); )
				{
					if(compIDs.contains(itr->first))
					{
						itr = outComponents.erase(itr);
						compIDs.erase(itr->first);
					}
					else {
						++itr;
					}
				}

				// find this entity new home
				archetype = manager.CreateArchetype(Vector<ComponentID>(compIDs.begin(), compIDs.end()));
				archetype->InsertNoSort(entityID, std::move(outComponents));

				manager.m_EntityToArchetype[entityID] = archetype;
			};

			// If not update just remove from archetype
			if(!IsUpdating())
			{
				removeFunc(*this);
				return;
			}

			// else find if this entity is in the await collection, if so then remove it
			if(auto awaitItr{ m_AwaitEntitiesCollection.find(entityID) }; awaitItr != m_AwaitEntitiesCollection.end())
			{
				const auto compIDs{ MakeComponentSet<Args...>() };
				for(auto itr = awaitItr->second.begin(); itr != awaitItr->second.end(); )
				{
					if(compIDs.contains(itr->first))
					{
						itr = awaitItr->second.erase(itr);
						compIDs.erase(itr->first);
					}
					else {
						++itr;
					}
				}

				return;
			}

			// else add to await actions for late remove
			m_AwaitActions.emplace_back(std::move(removeFunc));
		}

		void Remove(EntityID entityID) noexcept;

		template <typename ... Args> requires (ComponentConcept<Args> && ...)
		SharedPtr<Archetype> CreateArchetype()
		{
			const Vector<ComponentID> componentRange{ MakeComponentVec<Args...>() };
			
			if(Contains(componentRange))
			{
				return GetArchetypeFrom(componentRange);
			}

			const AllocatorContext context{ AllocatorScope::Game };

			auto* upStream{ context.GetResource() };

			const SharedPtr<Archetype> archetype{
				std::allocate_shared<Archetype>(upStream, componentRange)};


			return archetype;
		}

		SharedPtr<Archetype> CreateArchetype(const Vector<ComponentID>& components)
		{
			if(Contains(components))
			{
				return GetArchetypeFrom(components);
			}

			const AllocatorContext context{ AllocatorScope::Game };

			auto* upStream{ context.GetResource() };

			const SharedPtr<Archetype> archetype{
				std::allocate_shared<Archetype>(upStream, components)};

			return archetype;
		}

		

	
	private:
	
		void InsertArchetype(const SharedPtr<Archetype>& archetype);

		IndexedMultimap<SharedPtr<Archetype>> m_Archetypes;
 
		Vector<std::function<void(ECSManager&)>> m_AwaitActions;
		UnOrderedMap<EntityID, ComponentStorage> m_AwaitEntitiesCollection;

		UnOrderedMap<EntityID, SharedPtr<Archetype>> m_EntityToArchetype;
		std::atomic<EntityID> m_CurrentEntityID{};
		bool m_IsUpdating{};

	};


}

