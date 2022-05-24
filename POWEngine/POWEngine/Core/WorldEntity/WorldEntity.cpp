#include "pch.h"
#include "WorldEntity.h"

#include "POWEngine/ECS/Archetype.h"
#include "POWEngine/ECS/SystemBase.h"

powe::WorldEntity::WorldEntity()
	: m_SparseComponentManager(*this)
{
}

void powe::WorldEntity::RegisterGameObject(GameObjectID id)
{
	if (!m_GameObjectRecords.contains(id))
		m_GameObjectRecords.try_emplace(id, GameObjectRecord{});
}

powe::WorldEntity::~WorldEntity()
{
	ClearArchetype();
}

void powe::WorldEntity::AddSystem(PipelineLayer layer, const SharedPtr<SystemBase>& system)
{
	system->SetWorld(this);
	m_PendingAddSystem.Push(SystemTrait{ system,layer });
}

void powe::WorldEntity::RemoveSystem(PipelineLayer layer, const SharedPtr<SystemBase>& system)
{
	m_PendingDeleteSystem.Push(SystemTrait{ system,layer });
}

powe::GameObjectID powe::WorldEntity::GetNewEntityID()
{
	return m_GameObjectCounter++;
}

void powe::WorldEntity::RemoveComponentByID(GameObjectID id, ComponentTypeID componentID)
{
	GameObjectRecord gbRecords{};

	if (GetGameObjectRecords(id, gbRecords))
	{
		// if this gameobject has the archetype that means it got registered in the pipeline
		if (const auto oldArchetype{ gbRecords.Archetype.lock() })
		{
			AddComponentToGameObjectRemoveList(id, componentID);
		}
		else
		{
			//SharedPtr<SparseComponent> some{new SparseComponent()}
			// find the component id in the pre-archetype list and remove it instantly
			RemoveComponentFromPreArchetype(id, componentID);
		}

	}

#pragma region OLDCODE

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

#pragma endregion

}

void powe::WorldEntity::RemoveGameObject(GameObjectID id, bool removeRecord)
{
	GameObjectRecord gbRecords{};
	if (GetGameObjectRecords(id, gbRecords))
	{
		// GameObject can only be deleted if it's in the pipeline
		const auto archetype{ gbRecords.Archetype.lock() };
		if (archetype)
		{
			AddGameObjectToArchetypeRemoveList(CreateStringFromNumVector(archetype->Types), id);
		}
		else
		{
			// Remove all associated components in pre archetype
			RemoveGameObjectFromPreArchetype(id);
		}

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
		for (const auto& archetype : m_ArchetypesPool | std::views::values)
		{
			if (!archetype->GameObjectIds.empty())
			{
				if (IsDigitExistInNumber(archetype->ComponentOffsets, system->GetKeys()))
					system->InternalUpdate(*archetype, deltaTime);
			}
		}
	}
}

void powe::WorldEntity::ResolveEntities()
{
	InternalRemoveGameObjectFromPipeline();
	InternalRemoveComponentFromGameObject();
	InternalAddGameObjectToPipeline();
	InternalAddSystemToPipeline();
	InternalRemoveSystemFromPipeline();
	ClearEmptyArchetype();
}

SharedPtr<powe::Archetype> powe::WorldEntity::GetArchetypeByGameObject(GameObjectID id) const
{
	if (m_GameObjectRecords.contains(id))
	{
		return m_GameObjectRecords.at(id).Archetype.lock();
	}

	return nullptr;
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
	for (const auto& gameObjectIDs : m_PendingDeleteGameObjectsFromArchetype | std::views::values)
	{
		for (int deletingGameObjectIdx = 0; deletingGameObjectIdx < int(gameObjectIDs.size()); ++deletingGameObjectIdx)
		{
			GameObjectRecord gbRecords{};

			// No need to check for null at this point
			GetGameObjectRecords(gameObjectIDs[deletingGameObjectIdx], gbRecords);

			const auto targetArchetype{ gbRecords.Archetype.lock() };
			const int thisGameObjectIndex{ gbRecords.IndexInArchetype };

			// 1. Destroy old component data
			{
				//RawByte* startAddress{ &targetArchetype->ComponentData[int(thisGameObjectIndex * targetArchetype->SizeOfComponentsBlock)] };
				//SizeType accumulateOffset{};
				//for (const auto& componentID : targetArchetype->Types)
				//{
				//	const SharedPtr<BaseComponent> componentTrait{ GetComponentTrait(componentID) };

				//	if (IsThisComponentSparse(componentID))
				//	{
				//		m_SparseComponentManager.RemoveComponentFromGameObject(*this, gameObjectIDs[deletingGameObjectIdx], componentID);
				//	}
				//	else
				//		componentTrait->DestroyData(startAddress + accumulateOffset);

				//	accumulateOffset += componentTrait->GetSize();
				//}

				DestroyComponentData(*targetArchetype, thisGameObjectIndex, gameObjectIDs[deletingGameObjectIdx], targetArchetype->Types);
			}


			// 2. Shift the data from everything below this GameObject until the next id that needed to be destroy
			if (thisGameObjectIndex < int(targetArchetype->GameObjectIds.size()))
			{
				// if there's a gameobject that suppose to be destroy next then we early stop the move data and
				// skip to the next block
				const GameObjectID stopGameObjectID{
					gameObjectIDs[(deletingGameObjectIdx + 1) % int(gameObjectIDs.size())] };

				const GameObjectID startGameObjectID{ targetArchetype->GameObjectIds[thisGameObjectIndex] };

				for (int i = thisGameObjectIndex; i < int(targetArchetype->GameObjectIds.size() - 1); ++i)
				{
					const GameObjectID thisGameObjectID{ targetArchetype->GameObjectIds[i] };

					if (thisGameObjectID == stopGameObjectID &&
						stopGameObjectID != startGameObjectID)
						break;

					SizeType accumulateOffset{};
					RawByte* toAddress{ &targetArchetype->ComponentData[int(i * targetArchetype->SizeOfComponentsBlock)] };
					RawByte* fromAddress{ &targetArchetype->ComponentData[int((i + 1) * targetArchetype->SizeOfComponentsBlock)] };

					for (const auto& componentID : targetArchetype->Types)
					{
						const SharedPtr<BaseComponent> componentTrait{ GetComponentTrait(componentID) };

						componentTrait->MoveData(fromAddress + accumulateOffset,
							toAddress + accumulateOffset);

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
		GameObjectRecord gbRecords{};

		if (!GetGameObjectRecords(gameObjectID, gbRecords))
			continue;

		const auto oldArchetype{ gbRecords.Archetype.lock() };
		if (!oldArchetype)
			continue;

		std::vector<ComponentTypeID> newTypes{ oldArchetype->Types };

		// remove every element that matches the deleting componentIDs
		newTypes.erase(std::ranges::remove_if(newTypes, [&componentIDs](ComponentTypeID id)
			{
				return std::ranges::find_if(componentIDs, [&id](ComponentTypeID removeID)
					{
						// discard the flag of the component
						return removeID == DiscardFlag(id);
					}) != componentIDs.end();
			}).begin(), newTypes.end());

		const std::string newArchetypeKey{ CreateStringFromNumVector(newTypes) };
		const std::string oldArchetypeKey{ CreateStringFromNumVector(oldArchetype->Types) };

		auto targetArchetype{ GetArchetypeFromActiveList(newArchetypeKey) };

		// 1. if the archetype is valid and is not the same archetype then move the remaining components to another archetype
		if (targetArchetype && targetArchetype != oldArchetype)
		{

			if (const SizeType targetOccupiedSize{ SizeType((targetArchetype->GameObjectIds.size() + 1) * targetArchetype->SizeOfComponentsBlock) };
				targetOccupiedSize >= targetArchetype->TotalAllocatedData)
			{
				// allocate more space if the current size is equal or more that the total allocated space
				targetArchetype->AllocateComponentData(SizeType(targetOccupiedSize * 3), *this);
			}

			RawByte* fromAddress{ &oldArchetype->ComponentData[int(gbRecords.IndexInArchetype * oldArchetype->SizeOfComponentsBlock)] };
			RawByte* toAddress{ &targetArchetype->ComponentData[int(targetArchetype->GameObjectIds.size() * targetArchetype->SizeOfComponentsBlock)] };

			for (const ComponentTypeID componentTypeId : newTypes)
			{
				const SharedPtr<BaseComponent> componentTrait{ GetComponentTrait(componentTypeId) };
				componentTrait->MoveData(
					fromAddress + oldArchetype->ComponentOffsets.at(componentTypeId),
					toAddress + targetArchetype->ComponentOffsets.at(componentTypeId));
			}
		}
		else
		{
			// 2. Else we need to create a new archetype and copy over all the data that we need
			//targetArchetype = CreateArchetype(newTypes);

			//SizeType offset{};
			//RawByte* fromAddress{ &oldArchetype->ComponentData[int(gbRecords.IndexInArchetype * oldArchetype->SizeOfComponentsBlock)] };
			//RawByte* toAddress{ &targetArchetype->ComponentData[0] };

			//for (const ComponentTypeID componentTypeId : newTypes)
			//{
			//	const SharedPtr<BaseComponent> thisComponent{ GetComponentTrait(componentTypeId) };

			//	thisComponent->MoveData(fromAddress + offset, toAddress + offset);

			//	offset += thisComponent->GetSize();
			//}

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

		// 3. Destroy the component and move over all the data after this gameobject

		DestroyComponentData(*oldArchetype, gbRecords.IndexInArchetype, gameObjectID, componentIDs);
		oldArchetype->BuryBlock(*this, gbRecords.IndexInArchetype);

		oldArchetype->GameObjectIds.erase(
			std::ranges::remove(oldArchetype->GameObjectIds, gameObjectID).begin(),
			oldArchetype->GameObjectIds.end());
	}

	m_PendingDeleteComponentsFromGameObject.clear();

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

void powe::WorldEntity::ClearArchetype()
{
	for (const auto& archetype : m_ArchetypesPool | std::views::values)
	{
		archetype->CleanUp(*this);
	}
}

void powe::WorldEntity::ClearEmptyArchetype()
{
	for (auto it = m_ArchetypesPool.begin(); it != m_ArchetypesPool.end(); ++it)
	{
		if (it->second->GameObjectIds.empty())
			it->second->CleanUp(*this);
	}
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
		std::vector<GameObjectID> temp{};
		temp.emplace_back(id);
		//m_PendingDeleteGameObjectsFromArchetype[archetypeKey] = temp;
		m_PendingDeleteGameObjectsFromArchetype.try_emplace(archetypeKey, temp);
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
		std::vector<ComponentTypeID> temp{};
		temp.emplace_back(componentTypeID);
		//m_PendingDeleteComponentsFromGameObject[id] = temp;
		m_PendingDeleteComponentsFromGameObject.try_emplace(id, temp);
	}
}

void powe::WorldEntity::AddArchetype(const std::string& key, const SharedPtr<Archetype>& archetype)
{
	m_ArchetypesPool.try_emplace(key, archetype);
}

void powe::WorldEntity::AddPreArchetype(GameObjectID gameObjectID,
	ComponentTypeID componentID, const SharedPtr<RawByte[]>& reservedComponentData)
{
	const auto findItr{ m_PreArchetypes.find(gameObjectID) };
	if (findItr != m_PreArchetypes.end())
	{
		const auto& [_, result] = findItr->second.componentData.try_emplace(componentID, reservedComponentData);
		if (result)
			findItr->second.archetypeKey.emplace_back(componentID);
	}
	else
	{
		PreArchetypeTrait preArchetypeTrait{};
		preArchetypeTrait.archetypeKey.emplace_back(componentID);
		preArchetypeTrait.componentData.try_emplace(componentID, reservedComponentData);
		m_PreArchetypes.try_emplace(gameObjectID, preArchetypeTrait);
	}
}

void powe::WorldEntity::RemoveComponentFromPreArchetype(GameObjectID id, ComponentTypeID componentTypeId)
{
	const auto gameObjectItr{ m_PreArchetypes.find(id) };
	if (gameObjectItr != m_PreArchetypes.end())
	{
		PreArchetypeTrait& archetypeTrait{ gameObjectItr->second };
		if (archetypeTrait.componentData.contains(componentTypeId))
		{
			archetypeTrait.componentData.erase(componentTypeId);

			archetypeTrait.archetypeKey.erase(
				std::ranges::remove(archetypeTrait.archetypeKey, componentTypeId).begin(),
				archetypeTrait.archetypeKey.end());
		}
	}
}

void powe::WorldEntity::RemoveGameObjectFromPreArchetype(GameObjectID id)
{
	if (m_PreArchetypes.contains(id))
	{
		m_PreArchetypes.erase(id);
	}
}

void powe::WorldEntity::DestroyComponentData(
	const Archetype& archetype, int index, GameObjectID id,
	const std::vector<ComponentTypeID>& components)
{
	RawByte* startAddress{ &archetype.ComponentData[int(index * archetype.SizeOfComponentsBlock)] };

	for (const ComponentTypeID componentTypeId : components)
	{
		const SizeType offset{ archetype.ComponentOffsets.at(componentTypeId) };
		const SharedPtr<BaseComponent> componentTrait{ GetComponentTrait(componentTypeId) };

		if (archetype.ComponentOffsets.contains(componentTypeId)) // check if this component id is a sparse component
		{
			m_SparseComponentManager.RemoveComponentFromGameObject(id, componentTypeId);
		}
		else
			componentTrait->DestroyData(startAddress + offset);
	}
}


SharedPtr<powe::BaseComponent> powe::WorldEntity::GetComponentTrait(ComponentTypeID id) const
{
	if (id & int(ComponentFlag::Sparse))
		return SparseComponent::GetStatic();

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

bool powe::WorldEntity::GetGameObjectRecords(GameObjectID id, GameObjectRecord& outRecord) const
{
	if (m_GameObjectRecords.contains(id))
	{
		outRecord = m_GameObjectRecords.at(id);
		return true;
	}

	return false;
}

bool powe::WorldEntity::GetPreArchetypeTrait(GameObjectID id, PreArchetypeTrait& outTrait) const
{
	if (m_PreArchetypes.contains(id))
	{
		outTrait = m_PreArchetypes.at(id);
		return true;
	}

	return false;
}

void powe::WorldEntity::InternalAddGameObjectToPipeline()
{
	for (const auto& [gameObjectID, preArchetypeTrait] : m_PreArchetypes)
	{
		const auto& archetypeKey{ preArchetypeTrait.archetypeKey };
		const auto& componentTempDataMap{ preArchetypeTrait.componentData };

		const std::string archetypeKeyString{ CreateStringFromNumVector(archetypeKey) };

		bool shouldInitializeArchetype{};
		SharedPtr<Archetype> targetArchetype{ GetArchetypeFromActiveList(archetypeKeyString) };
		if (!targetArchetype)
		{
			targetArchetype = Archetype::Create(*this, archetypeKey);
			shouldInitializeArchetype = true;
			m_ArchetypesPool.try_emplace(archetypeKeyString, targetArchetype);
		}

		const SizeType futureSize{ SizeType(targetArchetype->GameObjectIds.size() + 1)
				* targetArchetype->SizeOfComponentsBlock };

		if (futureSize >= targetArchetype->TotalAllocatedData)
		{
			targetArchetype->AllocateComponentData(futureSize * 3, *this);
		}

		SizeType accumulateOffset{};
		for (const ComponentTypeID componentTypeId : archetypeKey)
		{
			const SharedPtr<BaseComponent> componentTrait{ GetComponentTrait(componentTypeId) };
			const SharedPtr<RawByte[]> compData{ componentTempDataMap.at(componentTypeId) };

			RawByte* destination{ &targetArchetype->ComponentData[int(
				(targetArchetype->GameObjectIds.size() * targetArchetype->SizeOfComponentsBlock) +
				accumulateOffset)] };

			// Check if this component is sparse or not
			if (IsThisComponentSparse(componentTypeId))
			{
				//const ComponentTypeID discardCompFlag{ componentTypeId & ~SizeType(ComponentFlag::Count) };
				m_SparseComponentManager.AddComponentToSparseSet(gameObjectID, DiscardFlag(componentTypeId), compData);

				// Initialize the handle
				new (destination) SparseComponent();
			}
			else
			{
				RawByte* source{ componentTempDataMap.at(componentTypeId).get() };
				componentTrait->MoveData(source, destination);
			}



			//if (shouldInitializeArchetype)
			//{
			//	targetArchetype->ComponentOffsets.try_emplace(componentTypeId, accumulateOffset);

			//	// safety block for duplicates
			//	if (std::ranges::find(targetArchetype->Types, componentTypeId) == targetArchetype->Types.end())
			//	{
			//		targetArchetype->Types.emplace_back(componentTypeId);
			//	}
			//}

			accumulateOffset += componentTrait->GetSize();
		}

		// add the record to this gameobject
		//GetGameObjectRecords(gameObjectID, gbRecord);
		GameObjectRecord gbRecord{};
		gbRecord.Archetype = targetArchetype;
		gbRecord.IndexInArchetype = int(targetArchetype->GameObjectIds.size());

		m_GameObjectRecords[gameObjectID] = gbRecord;

		targetArchetype->GameObjectIds.emplace_back(gameObjectID);

	}

	m_PreArchetypes.clear();
}

void powe::WorldEntity::InternalAddSystemToPipeline()
{
	while (!m_PendingAddSystem.Empty())
	{
		const auto systemTrait{ m_PendingAddSystem.Front() };
		m_PendingAddSystem.Pop();
		auto& existingSystems{ m_SystemPipeline[int(systemTrait.layer)] };
		if (std::ranges::find(existingSystems, systemTrait.system) == existingSystems.end())
		{
			const auto& system{ systemTrait.system };
			for (const auto& archetype : m_ArchetypesPool | std::views::values)
			{
				if (!archetype->GameObjectIds.empty())
				{
					if (IsDigitExistInNumber(archetype->ComponentOffsets, system->GetKeys()))
						system->InternalCreate(*archetype);
				}
			}

			existingSystems.emplace_back(systemTrait.system);
		}
	}

}

void powe::WorldEntity::InternalRemoveSystemFromPipeline()
{
	std::unordered_map<PipelineLayer, std::vector<SharedPtr<SystemBase>>> removingSystems{};

	while (!m_PendingDeleteSystem.Empty())
	{
		const auto systemTrait{ m_PendingDeleteSystem.Front() };
		m_PendingDeleteSystem.Pop();
		removingSystems[systemTrait.layer].emplace_back(systemTrait.system);
	}

	for (const auto& [layer, removeSystem] : removingSystems)
	{
		auto& existingSystems{ m_SystemPipeline[int(layer)] };

		const auto removeItr = std::ranges::remove_if(existingSystems, [&removeSystem](const SharedPtr<SystemBase>& system)
			{
				return std::ranges::find(removeSystem, system) != removeSystem.end();
			});

		for (auto it = removeItr.begin(); it != existingSystems.end(); ++it)
		{
			const auto& system{ it->get() };
			for (const auto& archetype : m_ArchetypesPool | std::views::values)
			{
				if (!archetype->GameObjectIds.empty())
				{
					if (IsDigitExistInNumber(archetype->ComponentOffsets, system->GetKeys()))
						system->InternalDestroy(*archetype);
				}
			}
		}

		existingSystems.erase(removeItr.begin(), existingSystems.end());
	}
}



