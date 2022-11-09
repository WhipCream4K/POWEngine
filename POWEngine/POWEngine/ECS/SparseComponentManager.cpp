#include "pch.h"
#include "SparseComponentManager.h"

#include <iostream>

#include "POWEngine/Core/WorldEntity/WorldEntity.h"

powe::SparseComponentManager::SparseComponentManager(WorldEntity& world)
	: m_WorldEntity(world)
{
}

void powe::SparseComponentManager::AddComponentToSparseSet(
	GameObjectID id, ComponentTypeID componentTypeId,
	const SharedPtr<RawByte[]>& data)
{
	if (m_GameObjectToHandle.contains(id) && m_GameObjectToHandle[id].contains(componentTypeId))
		return;

	const SharedPtr<BaseComponent> componentTrait{ m_WorldEntity.get().GetComponentTrait(componentTypeId) };
	const SizeType componentSize{ componentTrait->GetSize() };
	auto& sparseSet{ m_SparseComponentData[componentTypeId] };


	// if the data is depleted we move the whole block to a new allocated one
	if ((sparseSet.CurrentEmptyIndex + 1) * componentSize >= sparseSet.TotalAllocateData)
	{
		const SizeType newSize{ (((sparseSet.CurrentEmptyIndex + 1) * 2) * componentSize )};

		const SharedPtr<RawByte[]> newAddress{ SharedPtr<RawByte[]>(new RawByte[newSize]{}) };
		// move all the data over to a new one
		
		RawByte* sourceAddress{ sparseSet.Data.get() };
		RawByte* destination{ newAddress.get() };
		
		for (int i = 0; i < int(sparseSet.CurrentEmptyIndex); ++i)
		{
			const SizeType offset{ i * componentSize };
			componentTrait->MoveData(sourceAddress + offset, destination + offset);
		}

		sparseSet.Data = newAddress;
		sparseSet.TotalAllocateData = newSize;
	}

	// put the data in the array
	RawByte* sourceAddress{ data.get() };
	RawByte* endAddress{ &sparseSet.Data[int(sparseSet.CurrentEmptyIndex * componentSize)] };

	componentTrait->MoveData(sourceAddress, endAddress);

	//m_GameObjectToHandle[id].try_emplace(componentTypeId, sparseSet.CurrentEmptyIndex++);
	m_GameObjectToHandle[id][componentTypeId] = sparseSet.CurrentEmptyIndex++;

	if(std::ranges::find(sparseSet.GameObjectIDs,id) == sparseSet.GameObjectIDs.end())
	{
		sparseSet.GameObjectIDs.emplace_back(id);
	}
}

powe::RawByte* powe::SparseComponentManager::GetComponentData(GameObjectID id, ComponentTypeID compID,
	SizeType componentSize) const
{
	const auto findItr{ m_GameObjectToHandle.find(id) };
	if(findItr != m_GameObjectToHandle.end() && findItr->second.contains(compID))
	{
		const SparseHandle handle{ findItr->second.at(compID) };

		return &m_SparseComponentData.at(compID).Data[int(handle * componentSize)];
	}

	return nullptr;
}

void powe::SparseComponentManager::RemoveComponentFromGameObject(
	GameObjectID id,
	ComponentTypeID compID,
	bool callInternDestroy)
{
	const auto findItr{ m_GameObjectToHandle.find(id) };
	if(findItr != m_GameObjectToHandle.end() && findItr->second.contains(compID))
	{
		const SparseHandle handle{ findItr->second[compID] };

		auto& sparseSet{ m_SparseComponentData[compID] };

		const SharedPtr<BaseComponent> thisComponent{ m_WorldEntity.get().GetComponentTrait(compID) };
		const SizeType componentSize{ thisComponent->GetSize() };

		RawByte* sourceAddress{ &sparseSet.Data[int(handle * componentSize)] };

		if(callInternDestroy)
			thisComponent->InternalDestroy(sourceAddress,m_WorldEntity.get(), id);

		thisComponent->DestroyData(sourceAddress);

		// 1. Move data in sparse set up once block
		for (int i = int(handle); i < int(sparseSet.CurrentEmptyIndex - 1); ++i)
		{
			RawByte* toAddress{ &sparseSet.Data[i * int(componentSize)] };
			RawByte* fromAddress{ &sparseSet.Data[(i + 1) * int(componentSize)] };

			// Move data over to the new block
			thisComponent->MoveData(fromAddress, toAddress);
		}

		// 2. Reassign SparseHandle of every GameObject in this sparse set up 1 index
		if(handle < sparseSet.CurrentEmptyIndex)
		{
			for (int i = int(handle + 1); i < int(sparseSet.GameObjectIDs.size()); ++i)
			{
				const GameObjectID gameObject{ sparseSet.GameObjectIDs[i] };
				--m_GameObjectToHandle[gameObject].at(compID);
			}
		}

		const auto removeItr{ std::ranges::find(sparseSet.GameObjectIDs,id) };
		sparseSet.GameObjectIDs.erase(removeItr);

		m_GameObjectToHandle[id].erase(compID);

		--sparseSet.CurrentEmptyIndex;

	}
}


void powe::SparseComponentManager::RemoveGameObjectFromSparse(const std::vector<GameObjectID>& gameObjects)
{
	for (const auto& removeGameObjectsId : gameObjects)
	{
		removeGameObjectsId;
	}
}

powe::SparseComponentManager::~SparseComponentManager()
{
	for (const auto& [componentTypeID, sparseSet] : m_SparseComponentData)
	{
		const SharedPtr<BaseComponent> thisComponent{ m_WorldEntity.get().GetComponentTrait(componentTypeID) };
		const SizeType componentSize{ thisComponent->GetSize() };

		for (SparseHandle i = 0; i < sparseSet.GameObjectIDs.size(); ++i)
		{
			thisComponent->DestroyData(&sparseSet.Data[int(i * componentSize)]);
		}
	}
}
