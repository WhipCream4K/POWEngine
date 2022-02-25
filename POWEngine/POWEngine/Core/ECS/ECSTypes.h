#pragma once

namespace powe
{
	using ComponentTypeId = size_t;
	using GameObjectId = size_t;
	using RawByte = std::byte;

	struct Archetype;

	// POD 
	struct GameObjectInArchetypeRecord
	{
		WeakPtr<Archetype> Archetype{};
		int IndexInArchetype{ -1 };
	};
}
