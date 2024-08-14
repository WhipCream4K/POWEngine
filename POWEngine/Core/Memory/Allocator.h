#pragma once

#include <memory_resource>

#include "Utils/Service.h"


namespace powe
{
	class TrackableAllocator : public std::pmr::memory_resource
	{
	public:

		TrackableAllocator(std::pmr::memory_resource* upStream = std::pmr::new_delete_resource())
			: m_Upstream(upStream)
		{
		}

		TrackableAllocator(const TrackableAllocator&) = delete;
		TrackableAllocator& operator=(const TrackableAllocator&) = delete;
		TrackableAllocator(TrackableAllocator&&) noexcept = delete;
		TrackableAllocator& operator=(TrackableAllocator&&) noexcept = delete;

		virtual ~TrackableAllocator() override = default;

	protected:

		virtual void* do_allocate(std::size_t bytes, std::size_t alignment) override
		{
			void* ptr = m_Upstream->allocate(bytes, alignment);

#ifdef _DEBUG

			{
				std::scoped_lock lock(m_Mutex);
				m_AllocatedMemoryMap.try_emplace(ptr, bytes);
			}

			m_TotalAllocateMemory.fetch_add(bytes);
#endif


			return ptr;
		}

		virtual void do_deallocate(void* ptr, std::size_t bytes, std::size_t alignment) override
		{
#ifdef _DEBUG

			{
				std::scoped_lock lock(m_Mutex);
				auto it = m_AllocatedMemoryMap.find(ptr);
				if (it != m_AllocatedMemoryMap.end())
				{
					m_AllocatedMemoryMap.erase(it);
				}
			}

			m_TotalAllocateMemory.fetch_sub(bytes);
#endif
			m_Upstream->deallocate(ptr, bytes, alignment);
		}

		virtual bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override
		{
			return this == &other;
		}


		std::unordered_map<void*, size_t> m_AllocatedMemoryMap;
		std::atomic_size_t m_TotalAllocateMemory{ 0 };
		std::mutex m_Mutex; // for thread safety

		std::pmr::memory_resource* m_Upstream{};

	};
	
	template<typename T>
	struct AllocatorDeleter
	{
		
		std::pmr::memory_resource* allocator;

		AllocatorDeleter(std::pmr::memory_resource* upStream)
			: allocator(upStream)
		{
		}

		AllocatorDeleter() = default;
		
		void operator()(T* ptr) const noexcept
		{
			if constexpr (!std::is_void_v<T>)
			{
				ptr->~T();
				allocator->deallocate(ptr, sizeof(T), alignof(T));
			}
			else
			{
				allocator->deallocate(ptr, 0, alignof(std::max_align_t));
			}
		}
	};

	class Allocator : public IService<Allocator>, public std::pmr::memory_resource
	{
		
	};

}



