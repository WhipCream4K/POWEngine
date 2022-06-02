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
		std::vector<GameObjectID> GameObjectIDs;
		SizeType TotalAllocateData{};
		SharedPtr<RawByte[]> Data{};
	};

	class WorldEntity;
	class SparseComponentManager
	{

	public:

		SparseComponentManager(WorldEntity& world);

		void AddComponentToSparseSet(
			GameObjectID id,
			ComponentTypeID componentTypeId,
			const SharedPtr<RawByte[]>& data);

		template<typename T>
		RawByte* GetComponentData(GameObjectID id, ComponentTypeID compID) const;

		void RemoveComponentFromGameObject(
			GameObjectID id,
			ComponentTypeID compID
		);

		void RemoveGameObjectFromSparse(
			const std::vector<GameObjectID>& gameObjects
		);

		~SparseComponentManager();

	private:

		std::unordered_map<GameObjectID, std::unordered_map<ComponentTypeID, SparseHandle>> m_GameObjectToHandle;
		std::unordered_map<ComponentTypeID, SparseSet> m_SparseComponentData;
		RefWrap<WorldEntity> m_WorldEntity;
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


