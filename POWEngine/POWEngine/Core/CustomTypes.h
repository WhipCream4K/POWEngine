#pragma once

#include <any>
#include <memory>
#include <array>
#include <type_traits>

template<typename UserClass>
using SharedPtr = std::shared_ptr<UserClass>;

template<typename UserClass>
using WeakPtr = std::weak_ptr<UserClass>;

template<typename UserClass, typename Deleter = std::default_delete<UserClass>>
using OwnedPtr = std::unique_ptr<UserClass, Deleter>;

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

using Any = std::any;

namespace powe
{
	struct MessageBus
	{
		std::any data{};
		int size = 0;
		uint8_t eventId = 0;
	};
}
