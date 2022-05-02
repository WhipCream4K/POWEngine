#include "pch.h"
#include "WorldEntity.h"

#include "POWEngine/Core/ECS/Archetype.h"
#include "POWEngine/Core/ECS/SystemBase.h"

powe::WorldEntity::WorldEntity()
{
}

void powe::WorldEntity::RegisterGameObject(GameObjectID id)
{
	if (!m_GameObjectRecords.contains(id))
		m_GameObjectRecords.try_emplace(id, GameObjectInArchetypeRecord{});
}

void powe::WorldEntity::RemoveSystem(const SharedPtr<SystemBase>&) const
{
	//const auto& targetPipeline{ m_SystemPipeline[size_t(system->GetPipeLineLayer())] };
	//const auto& itr = std::ranges::find(targetPipeline, system); // ranges::find since c++20
	//if (itr != targetPipeline.end())
	//{
	//	itr->get()->MarkDeleted(true);
	//}
}

powe::GameObjectID powe::WorldEntity::GetNewEntityID()
{
	return m_GameObjectCounter++;
}

void powe::WorldEntity::RemoveComponentByID(GameObjectID id, ComponentTypeID componentID)
{
	GameObjectInArchetypeRecord gbRecords{};

	if (GetGameObjectRecords(id, gbRecords))
	{
		// if this gameobject has the archetype that means it got registered in the pipeline
		if(const auto oldArchetype{ gbRecords.Archetype.lock() })
		{
			AddComponentToGameObjectRemoveList(id, componentID);
		}
		else
		{
			// find the component id in the pre-archetype list and remove it instantly
			RemoveComponentFromPreArchetype(id, componentID);
		}

		//const std::string oldArchetypeKey{ CreateStringFromNumVector(oldArchetype->Types) };

		//std::vector<ComponentTypeID> newTypes{ oldArchetype->Types };
		//newTypes.erase(std::ranges::remove(newTypes, componentID).begin());

		//SharedPtr<Archetype> targetArchetype{};

		//if((targetArchetype = GetArchetypeFromActiveList(oldArchetypeKey)))
		//{
		//	// if there's an already active archetype, we need to told the world to remove the gameobject
		//	// from its archetype and find/create a new archetype for the gameobject the we just ejected.
		//	AddGameObjectToArchetypeRemoveList(oldArchetypeKey, id);
		//}

	}

	//const auto gameObjectItr{ m_GameObjectRecords.find(id) };
	//const auto componentItr{ m_ComponentTraitsMap.find(componentID) };

	//if (gameObjectItr != m_GameObjectRecords.end() && componentItr != m_ComponentTraitsMap.end())
	//{
	//	if (const SharedPtr<Archetype> oldArchetype{ gameObjectItr->second.Archetype.lock() })
	//	{
	//		// Construct a new archetype or append archetype that has the same type as this GameObject
	//		std::vector<ComponentTypeID> newTypes{ oldArchetype->Types };
	//		newTypes.erase(std::ranges::remove(newTypes, componentID).begin());

	//		const std::string archetypeKey{ CreateStringFromNumVector(newTypes) };

	//		//TODO: Fix where there's already an active archetype with the new key
	//		SharedPtr<Archetype> targetArchetype{ GetArchetypeFromPendingList(archetypeKey) };

	//		if (!targetArchetype)
	//		{
	//			targetArchetype = CreateArchetypeWithTypes(newTypes);
	//		}

	//		// Move Data from the old archetype to the new archetype
	//		if (const SizeType occupiedSize{ SizeType((targetArchetype->GameObjectIds.size() + 1) * targetArchetype->SizeOfComponentsBlock) };
	//			occupiedSize + targetArchetype->SizeOfComponentsBlock >= targetArchetype->TotalAllocatedData)
	//		{
	//			// allocate more space if the current size is equal or more that the total allocated space
	//			targetArchetype->AllocateComponentData(SizeType(occupiedSize * 3), *this);
	//		}

	//		SizeType accumulatedOffset{};
	//		const int startIndex{ int(gameObjectItr->second.IndexInArchetype * oldArchetype->SizeOfComponentsBlock) };
	//		const int endIndex{ int(targetArchetype->GameObjectIds.size() * targetArchetype->SizeOfComponentsBlock) };

	//		RawByte* startAddress{ &oldArchetype->ComponentData[startIndex] };
	//		RawByte* endAddress{ &targetArchetype->ComponentData[endIndex] };

	//		for (const auto& compID : oldArchetype->Types)
	//		{
	//			const SharedPtr<BaseComponent> thisComponent{ m_ComponentTraitsMap.at(compID) };

	//			if (compID == componentID)
	//			{
	//				accumulatedOffset += thisComponent->GetSize();
	//				continue;
	//			}

	//			thisComponent->MoveData(startAddress + accumulatedOffset, endAddress + accumulatedOffset);
	//			accumulatedOffset += thisComponent->GetSize();
	//		}

	//		RemoveGameObject(id, false);

	//		gameObjectItr->second.IndexInArchetype = int(targetArchetype->GameObjectIds.size());
	//		gameObjectItr->second.Archetype = targetArchetype;
	//		targetArchetype->GameObjectIds.emplace_back(id);
	//	}
	//}

}

void powe::WorldEntity::RemoveGameObject(GameObjectID id, bool removeRecord)
{
	GameObjectInArchetypeRecord gbRecords{};
	if (GetGameObjectRecords(id, gbRecords))
	{
		const auto archetype{ gbRecords.Archetype.lock() };
		AddGameObjectToArchetypeRemoveList(CreateStringFromNumVector(archetype->Types), id);

		if (removeRecord)
			AddGameObjectToRecordRemoveList(id);
	}

#pragma region OLDCODE

	//if (findItr != m_GameObjectRecords.end())
	//{
	//	if (const SharedPtr<Archetype> oldArchetype{ findItr->second.Archetype.lock() })
	//	{
	//		const std::string archetypeKey{ CreateStringFromNumVector(oldArchetype->Types) };
	//		const auto archetypeItr{ m_PendingAddArchetypes.find(archetypeKey) };

	//		const SizeType thisGameObjectIndex{ SizeType(findItr->second.IndexInArchetype) };

	//		SharedPtr<Archetype> targetArchetype{};

	//		targetArchetype = GetArchetypeFromPendingList(archetypeKey);

	//		if (!targetArchetype)
	//		{
	//			// we need to create a new archetype and add it to the pending list
	//			targetArchetype = CreateArchetypeWithTypes(oldArchetype->Types);
	//			*targetArchetype = targetArchetype->Copy(*oldArchetype, *this);
	//		}

	//		// 1. Destroy old component data
	//		{
	//			RawByte* startAddress{ &targetArchetype->ComponentData[int(thisGameObjectIndex * targetArchetype->SizeOfComponentsBlock)] };
	//			SizeType accumulateOffset{};
	//			for (const auto& componentID : targetArchetype->Types)
	//			{
	//				const SharedPtr<BaseComponent> thisComponent{ m_ComponentTraitsMap.at(componentID) };
	//				thisComponent->DestroyData(startAddress + accumulateOffset);
	//				accumulateOffset += m_ComponentTraitsMap.at(componentID)->GetSize();
	//			}
	//		}


	//		// 2. Shift the data from everything below this GameObject block to this block
	//		if (thisGameObjectIndex < targetArchetype->GameObjectIds.size())
	//		{
	//			//startAddress = &targetArchetype->ComponentData[SizeType((findItr->second.IndexInArchetype + 1) * int(targetArchetype->SizeOfComponentsBlock))];
	//			for (auto i = thisGameObjectIndex + 1; i < targetArchetype->GameObjectIds.size(); ++i)
	//			{
	//				SizeType accumulateOffset{};
	//				RawByte* startAddress{ &targetArchetype->ComponentData[int(i * targetArchetype->SizeOfComponentsBlock)] };
	//				RawByte* endAddress{ &targetArchetype->ComponentData[int((i - 1) * targetArchetype->SizeOfComponentsBlock)] };

	//				for (const auto& componentID : targetArchetype->Types)
	//				{
	//					const SharedPtr<BaseComponent> thisComponent{ m_ComponentTraitsMap.at(componentID) };
	//					thisComponent->MoveData(startAddress + accumulateOffset, endAddress + accumulateOffset);
	//					accumulateOffset += thisComponent->GetSize();
	//				}
	//			}
	//		}

	//		// 3. Remove the GameObject ID out of this archetype
	//		targetArchetype->GameObjectIds.erase(
	//			std::ranges::remove(targetArchetype->GameObjectIds, id).begin(),
	//			targetArchetype->GameObjectIds.end());

	//		if (removeRecord)
	//			m_GameObjectRecords.erase(findItr);

	//	}
	//}

#pragma endregion

}

void powe::WorldEntity::UpdatePipeline(PipelineLayer layer, float deltaTime)
{
	const auto& systemsInThisPipeline{ m_SystemPipeline[int(layer)] };

	for (const auto& system : systemsInThisPipeline)
	{
		for (const auto& [_, archetype] : m_ArchetypesPool)
		{
			if (!archetype->GameObjectIds.empty())
			{
				if (IsDigitExistInNumber(archetype->Types, system->GetKeys()))
					system->InternalUpdate(*archetype, deltaTime);
			}
		}
	}
}

SharedPtr<powe::BaseComponent> powe::WorldEntity::GetBaseComponentByID(ComponentTypeID id) const
{
	if (m_ComponentTraitsMap.contains(id))
		return m_ComponentTraitsMap.at(id);

	return nullptr;
}

SharedPtr<powe::Archetype> powe::WorldEntity::GetArchetypeByGameObject(GameObjectID id) const
{
	if (m_GameObjectRecords.contains(id))
	{
		return m_GameObjectRecords.at(id).Archetype.lock();
	}

	return nullptr;
}

bool powe::WorldEntity::IsDigitExistInNumber(const std::vector<ComponentTypeID>& compIds,
	const std::unordered_set<ComponentTypeID>& digit)
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

SharedPtr<powe::Archetype> powe::WorldEntity::CreateArchetypeWithTypes(const std::vector<ComponentTypeID>& typeID)
{
	// assume that the given keys is sorted
	const std::string key{ CreateStringFromNumVector(typeID) };

	if (!m_PendingAddArchetypes.contains(key))
	{
		const SharedPtr<Archetype> archetype{ std::make_shared<Archetype>() };

		archetype->Types = typeID;

		SizeType componentAccumulateSize{};
		for (const auto& componentID : typeID)
		{
			archetype->ComponentOffsets[componentID] = componentAccumulateSize;
			componentAccumulateSize += m_ComponentTraitsMap.at(componentID)->GetSize();
		}

		archetype->SizeOfComponentsBlock = componentAccumulateSize;
		m_PendingAddArchetypes[key] = archetype;
	}

	return m_PendingAddArchetypes[key];
}

std::string powe::WorldEntity::CreateStringFromNumVector(const std::vector<ComponentTypeID>& numList)
{
	std::string out{};
	for (const auto& val : numList)
	{
		out.append(std::to_string(val));
	}
	return out;
}

SharedPtr<powe::Archetype> powe::WorldEntity::UpdatePendingArchetypeKey(const std::string& targetKey,
	const std::string& newKey)
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


SharedPtr<powe::Archetype> powe::WorldEntity::GetArchetypeFromPendingList(const std::string& key)
{
	if (m_PendingAddArchetypes.contains(key))
		return m_PendingAddArchetypes[key];

	return nullptr;
}

void powe::WorldEntity::RemoveArchetype(const std::string& key)
{
	if (!m_PendingRemoveArchetypes.contains(key))
		m_PendingRemoveArchetypes.insert(key);
}

void powe::WorldEntity::InternalRemoveGameObjectFromPipeline()
{

#pragma region OLDCODE
	//const auto findItr{ m_GameObjectRecords.find(id) };

	//if (findItr != m_GameObjectRecords.end())
	//{
	//	if (const SharedPtr<Archetype> oldArchetype{ findItr->second.Archetype.lock() })
	//	{
	//		const std::string archetypeKey{ CreateStringFromNumVector(oldArchetype->Types) };
	//		const auto archetypeItr{ m_PendingAddArchetypes.find(archetypeKey) };

	//		const SizeType thisGameObjectIndex{ SizeType(findItr->second.IndexInArchetype) };

	//		SharedPtr<Archetype> targetArchetype{};

	//		targetArchetype = GetArchetypeFromPendingList(archetypeKey);

	//		if (!targetArchetype)
	//		{
	//			// we need to create a new archetype and add it to the pending list
	//			targetArchetype = CreateArchetypeWithTypes(oldArchetype->Types);
	//			*targetArchetype = targetArchetype->Copy(*oldArchetype, *this);
	//		}

	//		// 1. Destroy old component data
	//		{
	//			RawByte* startAddress{ &targetArchetype->ComponentData[int(thisGameObjectIndex * targetArchetype->SizeOfComponentsBlock)] };
	//			SizeType accumulateOffset{};
	//			for (const auto& componentID : targetArchetype->Types)
	//			{
	//				const SharedPtr<BaseComponent> thisComponent{ m_ComponentTraitsMap.at(componentID) };
	//				thisComponent->DestroyData(startAddress + accumulateOffset);
	//				accumulateOffset += m_ComponentTraitsMap.at(componentID)->GetSize();
	//			}
	//		}


	//		// 2. Shift the data from everything below this GameObject block to this block
	//		if (thisGameObjectIndex < targetArchetype->GameObjectIds.size())
	//		{
	//			//startAddress = &targetArchetype->ComponentData[SizeType((findItr->second.IndexInArchetype + 1) * int(targetArchetype->SizeOfComponentsBlock))];
	//			for (auto i = thisGameObjectIndex + 1; i < targetArchetype->GameObjectIds.size(); ++i)
	//			{
	//				SizeType accumulateOffset{};
	//				RawByte* startAddress{ &targetArchetype->ComponentData[int(i * targetArchetype->SizeOfComponentsBlock)] };
	//				RawByte* endAddress{ &targetArchetype->ComponentData[int((i - 1) * targetArchetype->SizeOfComponentsBlock)] };

	//				for (const auto& componentID : targetArchetype->Types)
	//				{
	//					const SharedPtr<BaseComponent> thisComponent{ m_ComponentTraitsMap.at(componentID) };
	//					thisComponent->MoveData(startAddress + accumulateOffset, endAddress + accumulateOffset);
	//					accumulateOffset += thisComponent->GetSize();
	//				}
	//			}
	//		}

	//		// 3. Remove the GameObject ID out of this archetype
	//		targetArchetype->GameObjectIds.erase(
	//			std::ranges::remove(targetArchetype->GameObjectIds, id).begin(),
	//			targetArchetype->GameObjectIds.end());

	//		if (shouldRemoveRecords)
	//			m_GameObjectRecords.erase(findItr);

	//	}
	//}
#pragma endregion

	// Deletes GameObject from archetype
	for (const auto& [archetypeKey, gameObjectIDs] : m_PendingDeleteGameObjectsFromArchetype)
	{
		for (int deletingGameObjectIdx = 0; deletingGameObjectIdx < int(gameObjectIDs.size()); ++deletingGameObjectIdx)
		{
			GameObjectInArchetypeRecord gbRecords{};

			// No need to check for null at this point
			GetGameObjectRecords(gameObjectIDs[deletingGameObjectIdx], gbRecords);

			const auto targetArchetype{ gbRecords.Archetype.lock() };
			const int thisGameObjectIndex{ gbRecords.IndexInArchetype };

			// 1. Destroy old component data
			{
				RawByte* startAddress{ &targetArchetype->ComponentData[int(thisGameObjectIndex * targetArchetype->SizeOfComponentsBlock)] };
				SizeType accumulateOffset{};
				for (const auto& componentID : targetArchetype->Types)
				{
					const SharedPtr<BaseComponent> componentTrait{ GetComponentTrait(componentID) };
					componentTrait->DestroyData(startAddress + accumulateOffset);
					accumulateOffset += componentTrait->GetSize();
				}
			}


			// 2. Shift the data from everything below this GameObject until the next id that needed to be destroy
			if (thisGameObjectIndex < int(targetArchetype->GameObjectIds.size()))
			{
				// if there's a gameobject that suppose to be destroy next then we early stop the move data and
				// skip to the next block
				const GameObjectID stopGameObjectID{
					gameObjectIDs[(deletingGameObjectIdx + 1) % int(gameObjectIDs.size())] };

				const GameObjectID startGameObjectID{ targetArchetype->GameObjectIds[thisGameObjectIndex] };

				for (int i = thisGameObjectIndex; i < int(targetArchetype->GameObjectIds.size()); ++i)
				{
					const GameObjectID thisGameObjectID{ targetArchetype->GameObjectIds[i] };

					if (thisGameObjectID == stopGameObjectID &&
						stopGameObjectID != startGameObjectID)
						break;

					SizeType accumulateOffset{};
					RawByte* startAddress{ &targetArchetype->ComponentData[int(i * targetArchetype->SizeOfComponentsBlock)] };
					RawByte* endAddress{ &targetArchetype->ComponentData[int((i - 1) * targetArchetype->SizeOfComponentsBlock)] };

					for (const auto& componentID : targetArchetype->Types)
					{
						const SharedPtr<BaseComponent> componentTrait{ GetComponentTrait(componentID) };

						componentTrait->MoveData(startAddress + accumulateOffset,
							endAddress + accumulateOffset);

						accumulateOffset += componentTrait->GetSize();
					}

				}

			}

			// 3. Remove the GameObject ID out of this archetype
			targetArchetype->GameObjectIds.erase(
				std::ranges::remove(targetArchetype->GameObjectIds, gameObjectIDs[deletingGameObjectIdx]).begin(),
				targetArchetype->GameObjectIds.end());
		}

	}

	// Deletes GameObjects From the records
	for (const auto deleteGameObjectID : m_PendingDeleteGameObjectsFromRecord)
	{
		m_GameObjectRecords.erase(deleteGameObjectID);
	}

	// Clear everything at the end of process
	m_PendingDeleteGameObjectsFromRecord.clear();
	m_PendingDeleteGameObjectsFromArchetype.clear();
}

void powe::WorldEntity::InternalRemoveComponentFromGameObject()
{
	for (const auto& [gameObjectID, componentIDs] : m_PendingDeleteComponentsFromGameObject)
	{
		GameObjectInArchetypeRecord gbRecords{};

		// No need to check for null at this point
		GetGameObjectRecords(gameObjectID, gbRecords);

		const auto oldArchetype{ gbRecords.Archetype.lock() };
		std::vector<ComponentTypeID> newTypes{ oldArchetype->Types };

		// remove every element that matches the deleting componentIDs
		newTypes.erase(std::ranges::remove_if(newTypes, [&componentIDs](ComponentTypeID id)
			{
				return std::ranges::find(componentIDs, id) != componentIDs.end();
			}).begin(), newTypes.end());

		const std::string newArchetypeKey{ CreateStringFromNumVector(newTypes) };
		const std::string oldArchetypeKey{ CreateStringFromNumVector(oldArchetype->Types) };

		// 1. Check if there's any archetype that has the same key
		if (const auto targetArchetype{ GetArchetypeFromActiveList(newArchetypeKey) })
		{
			// 1.1. if there's one then we can manipulate the data here directly

			if (const SizeType targetOccupiedSize{ SizeType((targetArchetype->GameObjectIds.size() + 1) * targetArchetype->SizeOfComponentsBlock) };
				targetOccupiedSize >= targetArchetype->TotalAllocatedData)
			{
				// allocate more space if the current size is equal or more that the total allocated space
				targetArchetype->AllocateComponentData(SizeType(targetOccupiedSize * 3), *this);
			}

			// copy data over to the new archetype
			
		}
		else
		{
			// 2. Else we need to create a new archetype and copy over all the data that we need
			for (const ComponentTypeID componentType : newTypes)
			{
				const SharedPtr<BaseComponent> oldComp{ GetComponentTrait(componentType) };
				SharedPtr<RawByte[]> tempComponentData{ SharedPtr<RawByte[]>(new RawByte[oldComp->GetSize()]{}) };

				// move data over
				{
					RawByte* oldStartDataAddress{ &oldArchetype->ComponentData[
						gbRecords.IndexInArchetype * int(oldArchetype->SizeOfComponentsBlock) +
						int(oldArchetype->ComponentOffsets.at(componentType))] };

					oldComp->MoveData(oldStartDataAddress, tempComponentData.get());
				}

				AddPreArchetype(gameObjectID, componentType, tempComponentData);
			}
		}

		AddGameObjectToArchetypeRemoveList(oldArchetypeKey, gameObjectID);

		//for (const auto deletingComponentID : componentIDs)
		//{

		//	newTypes.erase(std::ranges::remove(newTypes, deletingComponentID).begin());

		//	const std::string newArchetypeKey{ CreateStringFromNumVector(newTypes) };
		//	SharedPtr<Archetype> targetArchetype{ GetArchetypeFromActiveList(newArchetypeKey) };

		//	if(!targetArchetype)
		//	{
		//		// Create new archetype and copy the rest of the component data to the new archetype
		//		for (const auto & oldComponentType : oldArchetype->Types)
		//		{
		//			if(oldComponentType)
		//		}
		//	}
		//	
		//}
	}

	//const auto gameObjectItr{ m_GameObjectRecords.find(id) };
//const auto componentItr{ m_ComponentTraitsMap.find(componentID) };

//if (gameObjectItr != m_GameObjectRecords.end() && componentItr != m_ComponentTraitsMap.end())
//{
//	if (const SharedPtr<Archetype> oldArchetype{ gameObjectItr->second.Archetype.lock() })
//	{
//		// Construct a new archetype or append archetype that has the same type as this GameObject
//		std::vector<ComponentTypeID> newTypes{ oldArchetype->Types };
//		newTypes.erase(std::ranges::remove(newTypes, componentID).begin());

//		const std::string archetypeKey{ CreateStringFromNumVector(newTypes) };

//		//TODO: Fix where there's already an active archetype with the new key
//		SharedPtr<Archetype> targetArchetype{ GetArchetypeFromPendingList(archetypeKey) };

//		if (!targetArchetype)
//		{
//			targetArchetype = CreateArchetypeWithTypes(newTypes);
//		}

//		// Move Data from the old archetype to the new archetype
//		if (const SizeType occupiedSize{ SizeType((targetArchetype->GameObjectIds.size() + 1) * targetArchetype->SizeOfComponentsBlock) };
//			occupiedSize + targetArchetype->SizeOfComponentsBlock >= targetArchetype->TotalAllocatedData)
//		{
//			// allocate more space if the current size is equal or more that the total allocated space
//			targetArchetype->AllocateComponentData(SizeType(occupiedSize * 3), *this);
//		}

//		SizeType accumulatedOffset{};
//		const int startIndex{ int(gameObjectItr->second.IndexInArchetype * oldArchetype->SizeOfComponentsBlock) };
//		const int endIndex{ int(targetArchetype->GameObjectIds.size() * targetArchetype->SizeOfComponentsBlock) };

//		RawByte* startAddress{ &oldArchetype->ComponentData[startIndex] };
//		RawByte* endAddress{ &targetArchetype->ComponentData[endIndex] };

//		for (const auto& compID : oldArchetype->Types)
//		{
//			const SharedPtr<BaseComponent> thisComponent{ m_ComponentTraitsMap.at(compID) };

//			if (compID == componentID)
//			{
//				accumulatedOffset += thisComponent->GetSize();
//				continue;
//			}

//			thisComponent->MoveData(startAddress + accumulatedOffset, endAddress + accumulatedOffset);
//			accumulatedOffset += thisComponent->GetSize();
//		}

//		RemoveGameObject(id, false);

//		gameObjectItr->second.IndexInArchetype = int(targetArchetype->GameObjectIds.size());
//		gameObjectItr->second.Archetype = targetArchetype;
//		targetArchetype->GameObjectIds.emplace_back(id);
//	}
//}


}

void powe::WorldEntity::InternalAddArchetypeToPipeline()
{

}


void powe::WorldEntity::AddGameObjectToRecordRemoveList(GameObjectID id)
{
	if (std::ranges::find(m_PendingDeleteGameObjectsFromRecord, id) == m_PendingDeleteGameObjectsFromRecord.end())
		m_PendingDeleteGameObjectsFromRecord.emplace_back(id);
}

void powe::WorldEntity::AddGameObjectToArchetypeRemoveList(const std::string& archetypeKey, GameObjectID id)
{
	const auto findItr{ m_PendingDeleteGameObjectsFromArchetype.find(archetypeKey) };
	if (findItr != m_PendingDeleteGameObjectsFromArchetype.end())
	{
		auto& deleteGameObjectList{ findItr->second };
		if (std::ranges::find(deleteGameObjectList, id) == findItr->second.end())
			deleteGameObjectList.emplace_back(id);
	}
	else
	{
		std::vector<GameObjectID> temp{}; // i don't trust constructor
		temp.emplace_back(id);
		m_PendingDeleteGameObjectsFromArchetype[archetypeKey] = temp;
	}
}

void powe::WorldEntity::AddComponentToGameObjectRemoveList(GameObjectID id, ComponentTypeID componentTypeID)
{
	const auto findItr{ m_PendingDeleteComponentsFromGameObject.find(id) };
	if (findItr != m_PendingDeleteComponentsFromGameObject.end())
	{
		auto& deleteComponentList{ findItr->second };
		if (std::ranges::find(deleteComponentList, componentTypeID) == findItr->second.end())
			deleteComponentList.emplace_back(componentTypeID);
	}
	else
	{
		std::vector<ComponentTypeID> temp{}; // i don't trust constructor
		temp.emplace_back(id);
		m_PendingDeleteComponentsFromGameObject[id] = temp;
	}
}

void powe::WorldEntity::AddPreArchetype(GameObjectID gameObjectID,
	ComponentTypeID componentID, const SharedPtr<RawByte[]>& reservedComponentData)
{
	const auto findItr{ m_PendingAddArchetype.find(gameObjectID) };
	if (findItr != m_PendingAddArchetype.end())
	{
		findItr->second.componentData.try_emplace(componentID, reservedComponentData);
	}
}

void powe::WorldEntity::RemoveComponentFromPreArchetype(GameObjectID id, ComponentTypeID componentTypeId)
{
	const auto gameObjectItr{ m_PendingAddArchetype.find(id) };
	if(gameObjectItr != m_PendingAddArchetype.end())
	{
		if (gameObjectItr->second.componentData.contains(componentTypeId))
			gameObjectItr->second.componentData.erase(componentTypeId);
	}
}

powe::SizeType powe::WorldEntity::GetComponentSize(ComponentTypeID id) const
{
	// if the component id has a sparse flag then return handle size
	if (id & int(ComponentFlag::Sparse))
		return sizeof(SizeType);

	if (m_ComponentTraitsMap.contains(id))
	{
		return m_ComponentTraitsMap.at(id)->GetSize();
	}

	return {};
}

SharedPtr<powe::BaseComponent> powe::WorldEntity::GetComponentTrait(ComponentTypeID id) const
{
	if (id & int(ComponentFlag::Sparse))
		return StaticComponent::SparseTrait;

	if (m_ComponentTraitsMap.contains(id))
		return m_ComponentTraitsMap.at(id);

	return nullptr;
}

SharedPtr<powe::Archetype> powe::WorldEntity::GetArchetypeFromActiveList(const std::string& key) const
{
	if (m_ArchetypesPool.contains(key))
		return m_ArchetypesPool.at(key);

	return nullptr;
}

bool powe::WorldEntity::GetGameObjectRecords(GameObjectID id, GameObjectInArchetypeRecord& outRecord) const
{
	if (m_GameObjectRecords.contains(id))
	{
		outRecord = m_GameObjectRecords.at(id);
		return true;
	}

	return false;
}

//bool powe::WorldEntity::GetPendingArchetypeTrait(const std::string& archetypeKey, PreArchetypeTrait& outArchetypeTrait) const
//{
//	if (m_PendingAddArchetype.contains(archetypeKey))
//	{
//		outArchetypeTrait = m_PendingAddArchetype.at(archetypeKey);
//		return true;
//	}
//
//	return false;
//}


//SharedPtr<powe::Archetype> powe::WorldEntity::GetValidArchetype(const std::string& key)
//{
//	auto archetypeItr{ m_ArchetypesPool.find(key) };
//
//	if(archetypeItr == m_ArchetypesPool.end())
//	{
//		archetypeItr = m_PendingAddArchetypes.find(key);
//		if (archetypeItr == m_PendingAddArchetypes.end())
//			return nullptr;
//
//		return archetypeItr->second;
//	}
//
//	return archetypeItr->second;
//}

