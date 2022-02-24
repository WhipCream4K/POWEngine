#pragma once

#include "ECSTypes.h"
#include <vector>
#include "POWEngine/Core/CustomTypes.h"

namespace powe
{
	struct Archetype final
	{
		Archetype() = default;
		Archetype(const Archetype&) = default;
		Archetype& operator=(const Archetype&) = default;
		Archetype(Archetype&&) noexcept = default;
		Archetype& operator=(Archetype&&) noexcept = default;
		~Archetype() = default;

		std::vector<ComponentTypeId> Types; // types of components of this archetypes
		std::vector<GameObjectId> GameObjectIds; // GameObjectIds that has this archetype
		SharedPtr<uint8_t[]> ComponentData{}; // Array of component struct
		int SizeOfArchetype{};
		int TotalAllocateData{};
	};
}


