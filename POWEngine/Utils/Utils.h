#pragma once

#include "Core/CustomTypes.h"

namespace powe
{

	template<typename T, typename... Args>
	constexpr UniquePtr<T, AllocatorDeleter<T>> AllocateUnique(std::pmr::memory_resource* memResource, Args&&... args)
	{
		void* memory = memResource->allocate(sizeof(T), alignof(T));
		T* ptr = new (memory) T(std::forward<Args>(args)...);
		return UniquePtr<T, AllocatorDeleter<T>>(ptr, AllocatorDeleter<T>(memResource));
	}

	template<typename T>
	inline constexpr UniquePtr<T,AllocatorDeleter<T>> AllocateUnique(T&& object, std::pmr::memory_resource* memResource)
	{
        void* memory = memResource->allocate(sizeof(T), alignof(T));
		T* ptr{ new (memory) T(std::move(object)) };
        return UniquePtr<T, AllocatorDeleter<T>>(ptr, AllocatorDeleter<T>(memResource));
    }

	//template<typename T>
	//constexpr std::unique_ptr<T,AllocatorDeleter<T>> AllocateUnique(T&& object, std::pmr::memory_resource* memResource)
	//{
	//	using decayT = std::decay_t<T>;
	//	void* memory = memResource->allocate(sizeof(T), alignof(T));
	//	decayT* ptr{ new (memory) decayT(std::forward<T>(object)) };
	//	return std::unique_ptr<decayT, AllocatorDeleter<T>>(ptr, AllocatorDeleter<decayT>(memResource));
	//}
}
