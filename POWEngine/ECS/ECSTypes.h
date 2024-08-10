#pragma once

#include "Core/CustomTypes.h"
#include <unordered_map>

namespace powe
{
	using ComponentID = uint32_t;
	using EntityID = uint32_t;
	using SizeType = uint32_t;
	using SparseHandle = uint32_t;

	//constexpr ComponentTypeID ChildOf = (1u << 31u);
	constexpr EntityID INVALID_GAMEOBJECT_ID{ UINT32_MAX };

	// https://ajmmertens.medium.com/building-an-ecs-1-types-hierarchies-and-prefabs-9f07666a1e9d
	constexpr uint32_t SparseBitPos = 31;

	enum class ComponentFlag : ComponentID
	{
		Default,
		Sparse = (1u << 31u),

		Count = Sparse
	};
	
	struct SetHasher
	{
		size_t operator()(const Set<ComponentID>& other) const
		{
			std::size_t hash{};
			std::hash<uint32_t> hasher{};
			for (const auto& elem : other) {
				hash ^= hasher(elem) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
			}
			return hash;
		}
	};

	struct EqualOp
	{
		bool operator()(const Set<ComponentID>& left, const Set<ComponentID>& right) const
		{
			return std::ranges::all_of(right, [&left](const auto& elem) { return left.contains(elem); });
		}
	};

	template<typename Val>
	using ECSComponentMap = std::pmr::unordered_map<Set<ComponentID>, Val, SetHasher, EqualOp>;

	class BaseComponent;
	struct PreArchetypeTrait
	{
		ECSComponentMap<SharedPtr<RawByte[]>> componentData{};
		std::vector<ComponentID> archetypeKey{};
	};

	template<typename T>
	concept ComponentConcept = std::is_copy_constructible_v<T> && !std::is_pointer_v<T> && std::is_standard_layout_v<T>;

	using DynamicBitSet = std::vector<bool>;

	template<typename Val>
	using DynamicBitsetRange = Vector<std::pair<DynamicBitSet,Val>>;
	
}
