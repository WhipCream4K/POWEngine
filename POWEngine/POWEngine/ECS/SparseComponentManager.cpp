#include "pch.h"
#include "SparseComponentManager.h"

#include "POWEngine/Core/WorldEntity/WorldEntity.h"

powe::SparseComponentManager::SparseComponentManager(WorldEntity& world)
	: m_WorldEntity(world)
{
}

void powe::SparseComponentManager::AddComponentToSparseSet(
	const WorldEntity& world,
	GameObjectID id, ComponentTypeID componentTypeId,
	const SharedPtr<RawByte[]>& data)
{
	if (m_GameObjectToHandle.contains(id) && m_GameObjectToHandle[id].contains(componentTypeId))
		return;


	const SharedPtr<BaseComponent> componentTrait{ world.GetComponentTrait(componentTypeId) };
	const SizeType componentSize{ componentTrait->GetSize() };
	auto& sparseSet{ m_SparseComponentData[componentTypeId] };


	// if the data is depleted we move the whole block to a new allocated one
	if ((sparseSet.CurrentEmptyIndex + 1) * componentSize >= sparseSet.TotalAllocateData)
	{
		const SizeType newSize{ (sparseSet.CurrentEmptyIndex + 3) * componentSize };

		const SharedPtr<RawByte[]> newAddress{ SharedPtr<RawByte[]>(new RawByte[newSize]) };

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


	m_GameObjectToHandle[id].try_emplace(componentTypeId, sparseSet.CurrentEmptyIndex++);

	//m_RegisteredGameObjects.insert(id);
	//sparseSet.CurrentEmptyIndex++;
	//m_SparseHandleMap.try_emplace(id, sparseSet.CurrentEmptyIndex);
}

void powe::SparseComponentManager::RemoveComponentFromGameObject(const WorldEntity& worldEntity, 
	GameObjectID id,
	ComponentTypeID compID)
{
	const auto findItr{ m_GameObjectToHandle.find(id) };
	if(findItr != m_GameObjectToHandle.end() && findItr->second.contains(compID))
	{
		const SparseHandle handle{ findItr->second[compID] };

		auto& sparseSet{ m_SparseComponentData[compID] };

		const SharedPtr<BaseComponent> thisComponent{ worldEntity.GetComponentTrait(compID) };
		const SizeType componentSize{ thisComponent->GetSize() };

		RawByte* sourceAddress{ &sparseSet.Data[int(handle * componentSize)] };
		thisComponent->DestroyData(sourceAddress);

		for (int i = int(handle); i < int(sparseSet.CurrentEmptyIndex - 1); ++i)
		{
			RawByte* toAddress{ &sparseSet.Data[i * int(componentSize)] };
			RawByte* formAddress{ &sparseSet.Data[(i + 1) * int(componentSize)] };

			thisComponent->MoveData(formAddress, toAddress);
		}

		--sparseSet.CurrentEmptyIndex;

	}
}

powe::SparseComponentManager::~SparseComponentManager()
{
	for (const auto& [componentTypeID, sparseSet] : m_SparseComponentData)
	{
		const SharedPtr<BaseComponent> thisComponent{ m_WorldEntity.GetComponentTrait(componentTypeID) };
		const SizeType componentSize{ thisComponent->GetSize() };

		for (SparseHandle i = 0; i < sparseSet.CurrentEmptyIndex; ++i)
		{
			thisComponent->DestroyData(&sparseSet.Data[int(i * componentSize)]);
		}
	}
}
