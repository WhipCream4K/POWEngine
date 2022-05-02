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
		//ChildOf = (1u << 31u),
		Sparse = (1u << 31u)
	};


	struct Archetype;
	// POD 
	struct GameObjectInArchetypeRecord
	{
		WeakPtr<Archetype> Archetype{};
		int IndexInArchetype{ -1 };
	};

	class BaseComponent;
	struct PreArchetypeTrait
	{
		//using ComponentDataMap = std::unordered_map<ComponentTypeID, std::vector<SharedPtr<RawByte[]>>>;
		std::unordered_map<ComponentTypeID, SharedPtr<RawByte[]>> componentData{};
		std::string archetypeKey{};
	};

	//using PreArchetypeTrait = std::unordered_map<ComponentTypeID, SharedPtr<RawByte[]>>;
}
