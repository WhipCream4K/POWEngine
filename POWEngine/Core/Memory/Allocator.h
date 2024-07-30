#pragma once

#include <memory_resource>

#include "MemoryTracker.h"

namespace powe
{
	class MemoryTracker;
	class TrackableAllocator : public std::pmr::memory_resource
	{
	public:

		TrackableAllocator(std::pmr::memory_resource* upStream = std::pmr::new_delete_resource())
			: m_Upstream(upStream)
		{
		}

		TrackableAllocator(const TrackableAllocator&) = default;
		TrackableAllocator& operator=(const TrackableAllocator&) = default;
		TrackableAllocator(TrackableAllocator&&) noexcept = default;
		TrackableAllocator& operator=(TrackableAllocator&&) noexcept = default;

		virtual ~TrackableAllocator() = default;

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
			std::pmr::get_default_resource()->deallocate(ptr, bytes, alignment);
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
}



