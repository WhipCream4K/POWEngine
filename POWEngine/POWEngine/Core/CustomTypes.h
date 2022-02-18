#pragma once

#include <any>
#include <memory>
#include <array>
#include <type_traits>
#include <vector>

//#include "../Window/WindowContext.h"

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

template<typename FnType>
struct FnTraits{};

template<typename Ret,typename ...Args>
struct FnTraits<Ret(*)(Args...)> : FnTraits<Ret(Args...)>{};

template<typename Ret, typename ...Args>
struct FnTraits<Ret(Args...)>
{
	using return_type = Ret;
	using fn_type = Ret(Args...);
	using tuple_args = std::tuple<Args...>;
};

template<typename Ret, typename UserClass,typename ...Args>
struct FnTraits<Ret(UserClass::*)(Args...)> : FnTraits<Ret(UserClass&,Args...)>
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

#define UNREF_PARAM(param) param
