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

		static SharedPtr<Archetype> Create(const WorldEntity& world,const std::vector<ComponentTypeID>& types);

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

		RawByte* GetPointer(int pointerDiff) const;

		SharedPtr<RawByte[]> CopyComponentData(const Archetype& other, const WorldEntity& world) const;
		void BuryBlock(const WorldEntity& world,int index) const;
		void CleanUp(const WorldEntity& world) const;
		void AllocateComponentData(SizeType newSize, const WorldEntity& world);

		std::vector<ComponentTypeID> Types; // types of components of this archetypes
		std::vector<GameObjectID> GameObjectIds; // GameObjectIds that has this archetype
		ECSComponentMap<SizeType> ComponentOffsets;
		SharedPtr<RawByte[]> ComponentData{}; // Array of component struct
		SizeType SizeOfComponentsBlock{};
		SizeType TotalAllocatedData{};
	};
}


