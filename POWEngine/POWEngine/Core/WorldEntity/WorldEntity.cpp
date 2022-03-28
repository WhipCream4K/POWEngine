#include "pch.h"
#include "WorldEntity.h"

#include "POWEngine/Core/ECS/Archetype.h"
#include "POWEngine/Core/ECS/SystemBase.h"

powe::WorldEntity::WorldEntity() = default;

void powe::WorldEntity::RemoveSystem(const SharedPtr<SystemBase>&system) const
{
	const auto& targetPipeline{ m_SystemPipeline[size_t(system->GetPipeLineLayer())] };
	const auto& itr = std::ranges::find(targetPipeline, system); // ranges::find since c++20
	if (itr != targetPipeline.end())
	{
		itr->get()->MarkDeleted(true);
	}
}

powe::GameObjectId powe::WorldEntity::GetEntityId()
{
	return m_GameObjectCounter++;
}

void powe::WorldEntity::RemoveComponentByID(GameObjectId id, ComponentTypeID componentID)
{
	const auto gameObjectItr{ m_GameObjectRecords.find(id) };
	const auto componentItr{ m_ComponentMap.find(componentID) };

	if (gameObjectItr != m_GameObjectRecords.end() && componentItr != m_ComponentMap.end())
	{
		if (const SharedPtr<Archetype> oldArchetype{ gameObjectItr->second.Archetype.lock() })
		{

			// Construct a new archetype or append archetype that has the same type as this GameObject
			std::vector<ComponentTypeID> newTypes{ oldArchetype->Types };
			newTypes.erase(std::ranges::remove(newTypes, componentID).begin());

			const std::string archetypeKey{ CreateStringFromNumVector(newTypes) };

			SharedPtr<Archetype> targetArchetype{ GetArchetypeFromPendingList(archetypeKey) };

			if(!targetArchetype)
			{
				targetArchetype = CreateArchetypeWithKey(archetypeKey);
			}

			// Move Data from the old archetype to the new archetype
			if (const SizeType occupiedSize{ SizeType((targetArchetype->GameObjectIds.size() + 1) * targetArchetype->SizeOfComponentsBlock) };
				occupiedSize + targetArchetype->SizeOfComponentsBlock >= targetArchetype->TotalAllocatedData)
			{
				targetArchetype->AllocateComponentData(SizeType(occupiedSize * 3),*this);
			}

			SizeType accumulatedOffset{};
			const SizeType startIndex{ SizeType(gameObjectItr->second.IndexInArchetype * oldArchetype->SizeOfComponentsBlock) };
			const SizeType endIndex{ SizeType(targetArchetype->GameObjectIds.size() * targetArchetype->SizeOfComponentsBlock) };
			for (const auto& compID: oldArchetype->Types)
			{
				const SharedPtr<BaseComponent> thisComponent{ m_ComponentMap.at(compID) };

				if(compID == componentID)
				{
					accumulatedOffset += thisComponent->GetSize();
					continue;
				}

				RawByte* startAddress{ &oldArchetype->ComponentData[startIndex] };
				RawByte* endAddress{ &targetArchetype->ComponentData[endIndex] };
				thisComponent->MoveData(startAddress + accumulatedOffset, endAddress + accumulatedOffset);
				accumulatedOffset += thisComponent->GetSize();
			}

			RemoveGameObject(id,false);

			gameObjectItr->second.IndexInArchetype = int(targetArchetype->GameObjectIds.size());
			gameObjectItr->second.Archetype = targetArchetype;
			targetArchetype->GameObjectIds.emplace_back(id);
		}
	}

}

void powe::WorldEntity::RemoveGameObject(GameObjectId id, bool removeRecord)
{
	const auto findItr{ m_GameObjectRecords.find(id) };

	if (findItr != m_GameObjectRecords.end())
	{
		if (const SharedPtr<Archetype> oldArchetype{ findItr->second.Archetype.lock() })
		{
			const std::string archetypeKey{ CreateStringFromNumVector(oldArchetype->Types) };
			const auto archetypeItr{ m_PendingAddArchetypes.find(archetypeKey) };

			const SizeType thisGameObjectIndex{ SizeType(findItr->second.IndexInArchetype) };

			SharedPtr<Archetype> targetArchetype{};

			targetArchetype = GetArchetypeFromPendingList(archetypeKey);

			if (!targetArchetype)
			{
				// we need to create a new archetype and add it to the pending list
				targetArchetype = CreateArchetypeWithKey(archetypeKey);
				*targetArchetype = targetArchetype->Copy(*oldArchetype, *this);
			}

			// 1. Destroy old component data
			{
				RawByte* startAddress{ &targetArchetype->ComponentData[SizeType(thisGameObjectIndex * targetArchetype->SizeOfComponentsBlock)] };
				SizeType accumulateOffset{};
				for (const auto& componentID : targetArchetype->Types)
				{
					const SharedPtr<BaseComponent> thisComponent{ m_ComponentMap.at(componentID) };
					thisComponent->DestroyData(startAddress + accumulateOffset);
					accumulateOffset += m_ComponentMap.at(componentID)->GetSize();
				}
			}


			// 2. Shift the data from everything below this GameObject block to this block
			if (thisGameObjectIndex < targetArchetype->GameObjectIds.size())
			{
				//startAddress = &targetArchetype->ComponentData[SizeType((findItr->second.IndexInArchetype + 1) * int(targetArchetype->SizeOfComponentsBlock))];
				for (auto i = thisGameObjectIndex + 1; i < targetArchetype->GameObjectIds.size(); ++i)
				{
					SizeType accumulateOffset{};
					for (const auto& componentID : targetArchetype->Types)
					{
						const SharedPtr<BaseComponent> thisComponent{ m_ComponentMap.at(componentID) };
						RawByte* startAddress{ &targetArchetype->ComponentData[SizeType(i * targetArchetype->SizeOfComponentsBlock)] };
						RawByte* endAddress{ &targetArchetype->ComponentData[SizeType((i - 1) * targetArchetype->SizeOfComponentsBlock)] };
						thisComponent->MoveData(startAddress + accumulateOffset, endAddress + accumulateOffset);
						accumulateOffset += thisComponent->GetSize();
					}
				}
			}

			// 3. Remove the GameObject ID out of this archetype
			targetArchetype->GameObjectIds.erase(std::ranges::remove(targetArchetype->GameObjectIds, id).begin());

			if (removeRecord)
				m_GameObjectRecords.erase(findItr);

		}
	}
}

void powe::WorldEntity::UpdatePipeline(PipelineLayer layer, float deltaTime)
{
	const auto& systemsInThisPipeline{ m_SystemPipeline[size_t(layer)] };

	for (const auto& system : systemsInThisPipeline)
	{
		for (const auto& [_, archetype] : m_ArchetypesPool)
		{
			if (!archetype->GameObjectIds.empty())
			{
				if (IsDigitExistInNumber(archetype->Types, system->GetKeys()))
					system->InternalUpdate(*this, *archetype, deltaTime);
			}
			//if(!archetype->GameObjectIds.empty())
			//{
			//	if (IsDigitExistInNumber(archetype->Types, system->GetKeys()))
			//		system->InternalUpdate(*this, *archetype,deltaTime);
			//}
		}
	}
}

bool powe::WorldEntity::IsDigitExistInNumber(const std::vector<ComponentTypeID>&compIds,
	const std::unordered_set<ComponentTypeID>&digit)
{
	for (const auto& id : compIds)
	{
		// this checks also take into account the hierarchy of the components
		const ComponentTypeID number{ id & ~(1u << ChildOfBitPos) }; // clearing the child of bit flag
		if (!digit.contains(number))
			return false;
	}

	return true;
}

SharedPtr<powe::Archetype> powe::WorldEntity::CreateArchetypeWithTypes(const std::vector<ComponentTypeID>&typeID)
{
	// assume that the given keys is sorted
	const std::string key{ CreateStringFromNumVector(typeID) };

	if (!m_PendingAddArchetypes.contains(key))
	{
		const SharedPtr<Archetype> archetype{ std::make_shared<Archetype>() };
		m_PendingAddArchetypes[key] = archetype;
	}

	return m_PendingAddArchetypes[key];
}

SharedPtr<powe::Archetype> powe::WorldEntity::CreateArchetypeWithKey(const std::string & key)
{
	if (!m_PendingAddArchetypes.contains(key))
	{
		const SharedPtr<Archetype> archetype{ std::make_shared<Archetype>() };
		m_PendingAddArchetypes[key] = archetype;
	}

	return m_PendingAddArchetypes[key];
}

std::string powe::WorldEntity::CreateStringFromNumVector(const std::vector<ComponentTypeID>&numList)
{
	std::string out{};
	for (const auto& val : numList)
	{
		out.append(std::to_string(val));
	}
	return out;
}

SharedPtr<powe::Archetype> powe::WorldEntity::UpdatePendingArchetypeKey(const std::string & targetKey,
	const std::string & newKey)
{
	auto nodeHandle{ m_PendingAddArchetypes.extract(targetKey) };
	SharedPtr<Archetype> out{};

	if (!nodeHandle.empty())
	{
		nodeHandle.key() = newKey;
		out = nodeHandle.mapped();
		m_PendingAddArchetypes.insert(std::move(nodeHandle));
		return out;
	}

	return out;
}


SharedPtr<powe::Archetype> powe::WorldEntity::GetArchetypeFromPendingList(const std::string & key)
{
	if (m_PendingAddArchetypes.contains(key))
		return m_PendingAddArchetypes[key];

	return nullptr;
}

void powe::WorldEntity::RemoveArchetype(const std::string& key)
{
	if(!m_PendingRemoveArchetypes.contains(key))
	{
		m_PendingRemoveArchetypes.insert(key);
	}
}

SharedPtr<powe::Archetype> powe::WorldEntity::GetArchetypeFromActiveList(const std::string & key)
{
	if (m_ArchetypesPool.contains(key))
		return m_ArchetypesPool[key];

	return nullptr;
}

//SharedPtr<powe::Archetype> powe::WorldEntity::CreateAndAppendArchetypeByTypes(const std::vector<ComponentTypeID>& types)
//{
//	const std::string archetypeKey{ CreateStringFromNumVector(types) };
//	const auto archetypeItr{ m_ArchetypesPool.find(archetypeKey) };
//
//	SharedPtr<Archetype> outArchetype{};
// 
//	if(archetypeItr != m_ArchetypesPool.end())
//	{
//		const auto pendingItr{ m_PendingAddArchetypes.find(archetypeKey) };
//		if (pendingItr != m_PendingAddArchetypes.end())
//			outArchetype = pendingItr->second;
//	}
//	else
//	{
//		outArchetype = std::make_shared<Archetype>();
//	}
//
//	return outArchetype;
//}

