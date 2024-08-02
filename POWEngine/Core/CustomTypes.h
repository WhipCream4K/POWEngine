#pragma once

#include <any>
#include <memory>
#include <array>
#include <type_traits>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <memory_resource>
#include <string>

template<typename FnType>
struct FnTraits {};

template<typename Ret, typename ...Args>
struct FnTraits<Ret(*)(Args...)> : FnTraits<Ret(Args...)> {};

template<typename Ret, typename ...Args>
struct FnTraits<Ret(Args...)>
{
	using return_type = Ret;
	using fn_type = Ret(Args...);
	using tuple_args = std::tuple<Args...>;
};

template<typename Ret, typename UserClass, typename ...Args>
struct FnTraits<Ret(UserClass::*)(Args...)> : FnTraits<Ret(UserClass&, Args...)>
{
	using return_type = Ret;
	using fn_type = Ret(Args...);
	using tuple_args = std::tuple<Args...>;
	using class_type = UserClass;
};

template<typename Ret, typename UserClass, typename ...Args>
struct FnTraits<Ret(UserClass::*)(Args...) const> : FnTraits<Ret(UserClass&, Args...)>
{
	using return_type = Ret;
	using fn_type = Ret(Args...);
	using tuple_args = std::tuple<Args...>;
	using class_type = UserClass;
};

template<typename T, typename... Ts>
concept is_one_of = (std::is_same_v<T, Ts> || ...) || ( std::is_base_of_v<Ts, T> || ... );

namespace powe
{
	using RawByte = std::byte;

	template<typename UserClass>
	using SharedPtr = std::shared_ptr<UserClass>;

	template<typename T>
	using Vector = std::pmr::vector<T>;

	template<typename T>
	using List = std::pmr::list<T>;

	template<typename Key, typename T>
	using UnOrderedMap = std::pmr::unordered_map<Key, T>;

	template<typename UserClass>
	using WeakPtr = std::weak_ptr<UserClass>;

	template<typename T>
	struct AllocatorDeleter
	{

		std::pmr::memory_resource* allocator;

		AllocatorDeleter(std::pmr::memory_resource* upStream)
			: allocator(upStream)
		{
		}

		AllocatorDeleter() = default;

		void operator()(void* ptr)
		{
			if constexpr (!std::is_void_v<T>)
			{
				static_cast<T*>(ptr)->~T();
				allocator->deallocate(ptr, sizeof(T), alignof(T));
			}
			else
			{
				allocator->deallocate(ptr, 0, alignof(std::max_align_t));
			}
		}
	};

	template<typename UserClass, typename Deleter = AllocatorDeleter<UserClass>>
	using UniquePtr = std::unique_ptr<UserClass, Deleter>;

	template<typename T>
	using AlignedStorageT = std::aligned_storage_t<sizeof(T), alignof(T)>;

	template<bool val, typename ret = void>
	using EnableIf = std::enable_if_t<val, ret>;

	template<typename B, typename D, typename ret = void>
	using EnableIsBasedOf = std::enable_if_t<std::is_base_of_v<B, D>, ret>;

	template<typename T>
	using RemoveAllExt = std::remove_all_extents_t<T>;

	template<typename T>
	using PointTo = std::add_pointer_t<T>;

	template<typename T>
	using RefWrap = std::reference_wrapper<T>;
}
