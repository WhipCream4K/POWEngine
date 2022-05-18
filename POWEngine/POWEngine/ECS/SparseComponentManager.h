#pragma once

#include "ECSTypes.h"

namespace powe
{
	struct SparseSet
	{
		SparseSet()
			: CurrentEmptyIndex()
			, TotalAllocateData(8)
			, Data(SharedPtr<RawByte[]>(new RawByte[8]{}))
		{
		}

		SparseHandle CurrentEmptyIndex{};
		SizeType TotalAllocateData{};
		SharedPtr<RawByte[]> Data{};
	};

	class WorldEntity;
	class SparseComponentManager
	{

	public:

		SparseHandle AddComponentToSparseSet(
			const WorldEntity& world,
			GameObjectID id,
			ComponentTypeID componentTypeId,
			const SharedPtr<RawByte[]>& data);

		//RawByte* GetComponentData(GameObjectID id,ComponentTypeID compID,SparseHandle handle) const;

		template<typename T>
		RawByte* GetComponentData(GameObjectID id, ComponentTypeID compID, SparseHandle handle) const;

		void RemoveComponentFromGameObject(GameObjectID id, ComponentTypeID compID, SparseHandle handle);

	private:

		//std::unordered_map<GameObjectID, std::unordered_map<ComponentTypeID,SizeType>> m_SparseHandleMap;
		std::unordered_set<GameObjectID> m_RegisteredGameObjects;
		std::unordered_map<ComponentTypeID, SparseSet> m_SparseComponentData;
	};

	template <typename T>
	RawByte* SparseComponentManager::GetComponentData(GameObjectID id, ComponentTypeID compID,
		SparseHandle handle) const
	{
		if (!m_RegisteredGameObjects.contains(id))
			return nullptr;
		
		return &m_SparseComponentData.at(compID).Data[int(handle * sizeof(T))];
	}
}


