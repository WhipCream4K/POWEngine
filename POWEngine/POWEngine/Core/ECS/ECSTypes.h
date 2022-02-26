#pragma once

namespace powe
{
	using ComponentTypeId = uint32_t;
	using GameObjectId = uint32_t;
	using RawByte = std::byte;

	//constexpr ComponentTypeId ChildOf = (1u << 31u);

	// https://ajmmertens.medium.com/building-an-ecs-1-types-hierarchies-and-prefabs-9f07666a1e9d
	constexpr uint32_t ChildOfBitPos = 31;

	enum class ComponentFlag : ComponentTypeId
	{
		Default,
		ChildOf = (1u << 31u)
	};

	struct Archetype;

	// POD 
	struct GameObjectInArchetypeRecord
	{
		WeakPtr<Archetype> Archetype{};
		int IndexInArchetype{ -1 };
	};

	struct ArchetypeBuffer
	{
		ComponentTypeId CombinedId{};
		SharedPtr<Archetype> Archetype;
	};
}
