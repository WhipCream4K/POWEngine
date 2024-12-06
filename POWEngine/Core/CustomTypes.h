#pragma once

#include <any>
#include <memory>
#include <set>
#include <array>
#include <type_traits>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <memory_resource>
#include <functional>

#include "Core/Memory/Allocator.h"


template <typename T, typename... Ts>
concept is_one_of = (std::is_same_v<T, Ts> || ...) || (std::is_base_of_v<Ts, T> || ...);

// Helper struct to find the index of a type in a tuple
template <typename T, typename Tuple>
struct tuple_index;

template <typename T, typename... Types>
struct tuple_index<T, std::tuple<T, Types...>>
{
    static constexpr std::size_t value{};
};

template <typename T, typename U, typename... Types>
struct tuple_index<T, std::tuple<U, Types...>>
{
    static constexpr std::size_t value{tuple_index<T, std::tuple<Types...>>::value};
};

// Helper variable template
template <typename T, typename Tuple>
inline constexpr std::size_t tuple_index_v{tuple_index<T, Tuple>::value};

namespace powe
{
    //These two use a member function pointer type to deduce types for a callable (lambdas, mainly)
    template<typename T>
    struct MemFuncType
    {
        using type = void;
    };
    template<typename Ret, typename Class, typename... Args>
    struct MemFuncType<Ret (Class::*)(Args...) const>
    {
        using fn_type = std::function<Ret(Args...)>;
        using return_type = Ret;
        using arg_types = std::tuple<Args...>;
    };

    //Clean template to get the type info of a callable type (lambdas mainly)
    template<typename F>
    using FuncInfo = MemFuncType<decltype(&F::operator())>;


    using RawByte = std::byte;

    template<typename T>
    concept is_tuple = requires
    {
        typename std::tuple_size<T>::type;
    };
    
    template <typename UserClass>
    using SharedPtr = std::shared_ptr<UserClass>;

    template <typename T>
    using Vector = std::pmr::vector<T>;

    template <typename T>
    using List = std::pmr::list<T>;

    template <typename Key, typename T>
    using UnOrderedMap = std::pmr::unordered_map<Key, T>;

    template <typename T>
    using Set = std::pmr::set<T>;

    template <typename UserClass>
    using WeakPtr = std::weak_ptr<UserClass>;

    template <typename UserClass,typename Deleter = PolyMorphicDeleter>
    using UniquePtr = std::unique_ptr<UserClass,Deleter>;

    template <typename T>
    using AlignedStorageT = std::aligned_storage_t<sizeof(T), alignof(T)>;

    template <bool val, typename ret = void>
    using EnableIf = std::enable_if_t<val, ret>;

    template <typename B, typename D, typename ret = void>
    using EnableIsBasedOf = std::enable_if_t<std::is_base_of_v<B, D>, ret>;

    template <typename T>
    using RemoveAllExt = std::remove_all_extents_t<T>;

    template <typename T>
    using PointTo = std::add_pointer_t<T>;

    template <typename T>
    class RefWrap
    {
    public:
        
        explicit RefWrap(T& ref) : m_Ref(ref)
        {
        }
        
        T* operator->()
        {
            return &m_Ref.get();
        }

        T* operator->() const
        {
            return &m_Ref.get();
        }

        T& operator*() const
        {
            return m_Ref;
        }

        operator T&() const
        {
            return m_Ref.get();
        }

        operator const T&() const
        {
            return m_Ref.get();
        }

    private:
        
        std::reference_wrapper<T> m_Ref;
    };

    using PMRResource = std::pmr::memory_resource;
}
