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

	template<typename T, typename... Args>
	constexpr SharedPtr<T> AllocateShared(std::pmr::memory_resource* memResource, Args&&... args)
	{
		std::pmr::polymorphic_allocator<T> allocator(memResource);
		return std::allocate_shared<T>(allocator, std::forward<Args>(args)...);
	}

	bool IsInAppMainThread() noexcept;
	PMRResource* GetAppResource() noexcept;
}
