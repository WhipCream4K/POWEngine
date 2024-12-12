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
	// constexpr EntityID INVALID_GAMEOBJECT_ID{ UINT32_MAX };

	// // https://ajmmertens.medium.com/building-an-ecs-1-types-hierarchies-and-prefabs-9f07666a1e9d
	// constexpr uint32_t SparseBitPos = 31;

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
        	return std::includes(left.begin(), left.end(), right.begin(), right.end());
		}
	};

	template<typename Val>
	using ComponentSetMultimap = std::pmr::unordered_multimap<Set<ComponentID>, Val, SetHasher,EqualOp>;

	template<typename Val>
	using ComponentSetMap = std::pmr::unordered_map<Set<ComponentID>, Val, SetHasher,EqualOp>;

	// Check if a single type satisfies the conditions
	template <typename T>
	constexpr bool ComponentConditions = 
	    std::is_copy_constructible_v<T> &&
	    !std::is_pointer_v<T> &&
	    std::is_standard_layout_v<T>;

	// Check if all types in a parameter pack satisfy the conditions
	template <typename... Args>
	struct check_conditions_pack : std::conjunction<std::bool_constant<ComponentConditions<Args>>...> {};

	// Specialization for std::tuple
	template <typename Tuple>
	struct check_conditions_tuple;

	template <typename... Ts>
	struct check_conditions_tuple<std::tuple<Ts...>> : check_conditions_pack<Ts...> {};

	// Generic interface for both variadic arguments and tuples
	template <typename T>
	struct check_conditions;

	// For parameter packs (wrapped in tuple)
	template <typename... Args>
	struct check_conditions<std::tuple<Args...>> : check_conditions_pack<Args...> {};

	// For raw tuples
	template <typename T>
	struct check_conditions : check_conditions_tuple<T> {};

	// Helper variable template
	template <typename T>
	constexpr bool check_conditions_v = check_conditions<T>::value;

	// Component concept accepts type and also tuple packs
	template<typename T>
	concept ComponentConcept = check_conditions_v<std::decay_t<T>>;

	using CompAddress = void*;

	using ComponentStorage = Vector<std::pair<ComponentID,SharedPtr<void>>>;
}
