#pragma once

#include "Core/CustomTypes.h"

namespace powe
{

	template<typename T, typename... Args>
	constexpr UniquePtr<T> AllocateUnique(std::pmr::memory_resource* allocator, Args&&... args)
	{
		void* memory = allocator->allocate(sizeof(T), alignof(T));
		T* ptr = new (memory) T(std::forward<Args>(args)...);
		return UniquePtr<T,PolyMorphicDeleter>(ptr, allocator);
	}

	template<typename T, typename... Args>
	constexpr UniquePtr<T> AllocateUnique(const SharedPtr<PMRResource>& allocator, Args&&... args)
	{
		void* memory = allocator->allocate(sizeof(T), alignof(T));
		T* ptr = new (memory) T(std::forward<Args>(args)...);
		return UniquePtr<T,PolyMorphicDeleter>(ptr, allocator.get());
	}

	template<typename T>
	constexpr UniquePtr<T> AllocateUnique(T&& object, std::pmr::memory_resource* memResource)
	{
        void* memory = memResource->allocate(sizeof(T), alignof(T));
		T* ptr{ new (memory) T(std::move(object)) };

		return UniquePtr<T>(ptr, memResource);
    }

	
	bool IsInAppMainThread() noexcept;
	SharedPtr<PMRResource> GetAppResource() noexcept;
}
