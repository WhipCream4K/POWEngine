#pragma once

namespace powe
{
	using ComponentTypeID = uint32_t;
	using NormalIDType = ComponentTypeID; // This is for a normal use case of ID because I needed the same primitives as ComponentTypeID
	using GameObjectID = uint32_t;
	using RawByte = std::byte;
	using SizeType = uint32_t;

	//constexpr ComponentTypeID ChildOf = (1u << 31u);
	constexpr GameObjectID INVALID_GAMEOBJECT_ID{ UINT32_MAX };

	// https://ajmmertens.medium.com/building-an-ecs-1-types-hierarchies-and-prefabs-9f07666a1e9d
	constexpr uint32_t ChildOfBitPos = 31;

	enum class ComponentFlag : ComponentTypeID
	{
		Default,
		ChildOf = (1u << 31u),
		Sparse = (1u << 30u)
	};


	struct Archetype;
	// POD 
	struct GameObjectInArchetypeRecord
	{
		WeakPtr<Archetype> Archetype{};
		int IndexInArchetype{ -1 };
	};

	//struct ArchetypeTrait
	//{
	//	ComponentTypeID CombinedId{};
	//	SharedPtr<Archetype> Archetype{};
	//};
}
