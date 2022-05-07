#pragma once

#include "ECSTypes.h"

namespace powe
{
	struct SparseSet
	{
		SizeType CurrentEmptyIndex{};
		SizeType TotalAllocateData{};
		SharedPtr<RawByte[]> Data{};
	};

	

	class WorldEntity;
	class SparseComponentManager
	{

	public:

		SizeType AddComponentToSparseSet(
			const WorldEntity& world,
			GameObjectID id,
			ComponentTypeID componentTypeId,
			const SharedPtr<RawByte[]>& data);

		SharedPtr<RawByte[]> GetComponentData(GameObjectID id,ComponentTypeID compID);

	private:

		std::unordered_map<GameObjectID, std::vector<SizeType>> m_SparseHandleMap;
		std::unordered_map<ComponentTypeID, SparseSet> m_SparseComponentData;
		//std::unordered_map<ComponentTypeID,std::unordered_map<GameObjectID>>
	};
	
}


