#pragma once

#include <memory_resource>


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
			m_TotalAllocateMemory.fetch_add(bytes);
			void* ptr = m_Upstream->allocate(bytes, alignment);
			return ptr;
		}

		virtual void do_deallocate(void* ptr, std::size_t bytes, std::size_t alignment) override
		{
			m_TotalAllocateMemory.fetch_sub(bytes);
			m_Upstream->deallocate(ptr, bytes, alignment);
		}

		virtual bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override
		{
			return this == &other;
		}


		std::atomic_size_t m_TotalAllocateMemory{ 0 };

		std::pmr::memory_resource* m_Upstream{};

	};
	
	struct PolyMorphicDeleter
	{
		std::pmr::memory_resource* resource;

		PolyMorphicDeleter(std::pmr::memory_resource* upStream = std::pmr::get_default_resource())
			: resource(upStream)
		{
		}

		template<typename T>
		void operator()(T* ptr) const noexcept
		{
			ptr->~T();
			resource->deallocate(ptr, sizeof(T), alignof(T));
		}
	};

	template<typename T>
	struct AllocatorDeleter
	{
		
		std::pmr::memory_resource* resource;

		AllocatorDeleter(std::pmr::memory_resource* upStream)
			: resource(upStream)
		{
		}

		AllocatorDeleter() = default;
		
		template<typename U>
		void operator()(U* ptr) const noexcept
		{
			ptr->~U();
			resource->deallocate(ptr, sizeof(U), alignof(U));
		}
	};

}



