#pragma once

#include "POWEngine/Core/CustomTypes.h"
#include <unordered_map>

namespace powe
{
	using ComponentTypeID = uint32_t;
	using NormalIDType = ComponentTypeID; // This is for a normal use case of ID because I needed the same primitives as ComponentTypeID
	using GameObjectID = uint32_t;
	using SizeType = uint32_t;
	using SparseHandle = uint32_t;

	//constexpr ComponentTypeID ChildOf = (1u << 31u);
	constexpr GameObjectID INVALID_GAMEOBJECT_ID{ UINT32_MAX };

	// https://ajmmertens.medium.com/building-an-ecs-1-types-hierarchies-and-prefabs-9f07666a1e9d
	constexpr uint32_t SparseBitPos = 31;

	enum class ComponentFlag : ComponentTypeID
	{
		Default,
		Sparse = (1u << 31u),

		Count = Sparse
	};

	struct Archetype;
	// POD 
	struct GameObjectRecord
	{
		WeakPtr<Archetype> Archetype{};
		int IndexInArchetype{ -1 };
	};

	struct ComponentTypeHasher
	{
		size_t operator()(const ComponentTypeID& other) const
		{
			return std::hash<ComponentTypeID>()(other);
		}
	};

	struct ComponentEqualOp
	{
		bool operator()(const ComponentTypeID& left, const ComponentTypeID& right) const
		{
			return (left & ~SizeType(ComponentFlag::Count)) == right
				|| left == (right & ~SizeType(ComponentFlag::Count));
		}
	};

	template<typename Val>
	using ECSComponentMap = std::unordered_map<ComponentTypeID, Val, ComponentTypeHasher, ComponentEqualOp>;

	class BaseComponent;
	struct PreArchetypeTrait
	{
		ECSComponentMap<SharedPtr<RawByte[]>> componentData{};
		std::vector<ComponentTypeID> archetypeKey{};
	};

	//struct SparseHandle
	//{
	//	SizeType Index{};
	//	ComponentTypeID ComponentID{};
	//};

	//struct PreArchetypeTrait
	//{
	//	std::unordered_map<ComponentTypeID, SharedPtr<RawByte[]>> ComponentData{};
	//	GameObjectID OwnerID{};
	//};

	//struct PreArchetypeContainer
	//{
	//	std::unordered_map<std::string, std::vector<PreArchetypeTrait>> PreArchetypeTraits{};
	//	// This is to check whether the GameObject has been registered in the pipeline or not without iteratively check the map
	//	std::unordered_set<GameObjectID> GameObjectList{}; 
	//};

	//using PreArchetypeTrait = std::unordered_map<ComponentTypeID, SharedPtr<RawByte[]>>;
}
