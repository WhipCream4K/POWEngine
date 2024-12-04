#pragma once

#include "Utils/ThreadSafeSingleton.h"

namespace powe
{
	class TrackableAllocator;

	class MemoryManager : public ThreadSafeSingleton<MemoryManager>
	{
	public:

		MemoryManager(); 
		MemoryManager(const MemoryManager&) = delete;
		MemoryManager& operator=(const MemoryManager&) = delete;
		MemoryManager(MemoryManager&&) = delete;
		MemoryManager& operator=(MemoryManager&&) = delete;

		virtual ~MemoryManager() = default;

		// Pretty much just register the allocator that will use for creating new allocator 
		static void Init(const SharedPtr<PMRResource>& memResource);

		// SharedPtr<PMRResource> GetAllocator(std::string_view name) const;
		PMRResource* GetAllocator(std::string_view name) const noexcept;
		void RegisterAllocator(std::string_view name, PMRResource* allocator);
		void RegisterAllocator(std::string_view name, SharedPtr<PMRResource> allocator);

		// Provide a default allocator when called
		SharedPtr<PMRResource> NewAllocator(std::string_view name);
	
	private:

		UnOrderedMap<std::string, SharedPtr<PMRResource>> m_AllocatorMap;
		SharedPtr<PMRResource> m_Allocator;
	};

}



