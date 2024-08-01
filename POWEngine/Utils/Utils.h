#pragma once

//#include <SFML/Graphics/Color.hpp>
#include "Core/CustomTypes.h"
#include "Core/Memory/Allocator.h"

//namespace sf
//{
//    static sf::Color ConvertToSFColor(const glm::uvec4& color)
//    {
//        sf::Color sfColor{};
//        sfColor.r = sf::Uint8(color.r);
//        sfColor.g = sf::Uint8(color.g);
//        sfColor.b = sf::Uint8(color.b);
//        sfColor.a = sf::Uint8(color.a);
//
//        return sfColor;
//    }
//}

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
	constexpr UniquePtr<T,AllocatorDeleter<T>> AllocateUnique(T&& object, std::pmr::memory_resource* memResource)
	{
        void* memory = memResource->allocate(sizeof(T), alignof(T));
        T* ptr = new (memory) T(std::move(object));
        return UniquePtr<T, AllocatorDeleter<T>>(ptr, AllocatorDeleter<T>(memResource));
    }
}
