#include "pch.h"
#include "SparseComponentManager.h"

#include "POWEngine/Core/WorldEntity/WorldEntity.h"

powe::SizeType powe::SparseComponentManager::AddComponentToSparseSet(
	const WorldEntity& world,
	GameObjectID id, ComponentTypeID componentTypeId,
	const SharedPtr<RawByte[]>& data)
{
	if(m_SparseHandleMap.contains(id))
		return m_SparseComponentData[componentTypeId].CurrentEmptyIndex;
	
	const auto& [componentItr,result] = m_SparseComponentData.try_emplace(componentTypeId, SparseSet{});
	const SharedPtr<BaseComponent> componentTrait{ world.GetComponentTrait(componentTypeId) };
	auto& sparseSet{ componentItr->second };
	const SizeType componentSize{ componentTrait->GetSize() };

	// if the data is depleted we move the whole block to a new allocated one
	if((sparseSet.CurrentEmptyIndex + 1) * componentSize >= sparseSet.TotalAllocateData)
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
	RawByte* endAddress{ &sparseSet.Data[int(sparseSet.CurrentEmptyIndex * componentSize)]};

	componentTrait->MoveData(sourceAddress, endAddress);

	m_SparseHandleMap.try_emplace(id, sparseSet.CurrentEmptyIndex);

	return sparseSet.CurrentEmptyIndex++;
}

SharedPtr<powe::RawByte[]> powe::SparseComponentManager::GetComponentData(
	GameObjectID id, 
	ComponentTypeID compID)
{
	
}
