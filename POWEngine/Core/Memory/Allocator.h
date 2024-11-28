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
		std::mutex m_Mutex; // for thread safety

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

	struct DefaultAllocator
	{
		static PMRResource* Application;
		static PMRResource* Engine;

		static void ShutDown()
		{
			delete Application;
			delete Engine;
		}
	};

	

	// class GlobalAllocator : public ThreadSafeSingleton<GlobalAllocator>
	// {
	// public:
	//
	// 	static void InitDefaultAllocator() noexcept
	// 	{
	// 		auto* instance{Get()};
	// 		instance->CreateAllocator(DefaultAllocator::Application, new TrackableAllocator());
	// 		instance->CreateAllocator(DefaultAllocator::Engine, new TrackableAllocator());
	// 	}
	// 	
	// 	template<typename Alloc> requires std::is_base_of_v<PMRResource, std::decay_t<Alloc>>
	// 	Alloc* CreateAllocator(const std::string& name, Alloc* alloc) noexcept
	// 	{
	// 		m_AllocatorMap.try_emplace(name, std::unique_ptr<Alloc>(alloc,std::default_delete<Alloc>{}));
	// 		return m_AllocatorMap.at(name).get();
	// 	}
	//
	// 	PMRResource* GetAllocator(std::string_view name) // should throw exception if not found
	// 	{
	// 		auto it = m_AllocatorMap.find(name.data());
	// 		if (it != m_AllocatorMap.end())
	// 		{
	// 			return it->second.get();
	// 		}
	// 		return m_AllocatorMap.at(name.data()).get();
	// 	}
	// 	
	// private:
	//
	// 	std::unordered_map<std::string,std::unique_ptr<PMRResource>> m_AllocatorMap;
	// };

}



