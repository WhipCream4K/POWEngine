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

		void AddComponentToSparseSet(
			const WorldEntity& world,
			GameObjectID id,
			ComponentTypeID componentTypeId,
			const SharedPtr<RawByte[]>& data);


		template<typename T>
		RawByte* GetComponentData(GameObjectID id, ComponentTypeID compID) const;

		void RemoveComponentFromGameObject(
			const WorldEntity& worldEntity,
			GameObjectID id,
			ComponentTypeID compID
		);

	private:

		std::unordered_map<GameObjectID, std::unordered_map<ComponentTypeID, SparseHandle>> m_GameObjectToHandle;
		std::unordered_map<ComponentTypeID, SparseSet> m_SparseComponentData;
	};


	template <typename T>
	RawByte* SparseComponentManager::GetComponentData(GameObjectID id, ComponentTypeID compID) const
	{
		const auto findItr{ m_GameObjectToHandle.find(id) };
		if(findItr != m_GameObjectToHandle.end() && findItr->second.contains(compID))
		{
			const SparseHandle handle{ findItr->second.at(compID) };

			return &m_SparseComponentData.at(compID).Data[int(handle * sizeof(T))];
		}

		return nullptr;
	}
}


