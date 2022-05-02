#pragma once

#include "ECSTypes.h"
#include <vector>
#include "POWEngine/Core/CustomTypes.h"

namespace powe
{
	class WorldEntity;

	// POD for component data
	struct Archetype final
	{
		Archetype();
		Archetype(const Archetype&) = default;
		Archetype& operator=(const Archetype&) = default;
		Archetype(Archetype&&) noexcept = default;
		Archetype& operator=(Archetype&&) noexcept = default;
		~Archetype() = default;

		Archetype& Copy(const Archetype& other,const WorldEntity& world)
		{
			Types = other.Types;
			GameObjectIds = other.GameObjectIds;
			SizeOfComponentsBlock = other.SizeOfComponentsBlock;
			TotalAllocatedData = other.TotalAllocatedData;
			ComponentOffsets = other.ComponentOffsets;

			ComponentData = CopyComponentData(other,world);
			
			return *this;
		}

		SharedPtr<RawByte[]> CopyComponentData(const Archetype& other, const WorldEntity& world) const;
		//void CopyComponentData(const WorldEntity& world,const Archetype& other,int indexInArchetype);
		void AllocateComponentData(SizeType newSize, const WorldEntity& world);

		std::vector<ComponentTypeID> Types; // types of components of this archetypes
		std::vector<GameObjectID> GameObjectIds; // GameObjectIds that has this archetype
		std::unordered_map<ComponentTypeID,uint32_t> ComponentOffsets; // map of component's offsets from the start of the array
		SharedPtr<RawByte[]> ComponentData{}; // Array of component struct
		SizeType SizeOfComponentsBlock{};
		SizeType TotalAllocatedData{};
	};
}


