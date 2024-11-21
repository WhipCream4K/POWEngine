#pragma once

#include <mutex>

#include "Utils/Service.h"

namespace powe
{
	class TrackableAllocator;

	class MemoryManager : public IService<MemoryManager>
	{
	public:

		MemoryManager() = default;
		MemoryManager(const MemoryManager&) = delete;
		MemoryManager& operator=(const MemoryManager&) = delete;
		MemoryManager(MemoryManager&&) = delete;
		MemoryManager& operator=(MemoryManager&&) = delete;

		virtual ~MemoryManager() = default;

		PMRResource* GetAllocator(std::string_view name) const;
		SharedPtr<PMRResource> GetSharedAllocator(std::string_view name) const;
		void RegisterAllocator(std::string_view name, PMRResource* allocator);
		void RegisterAllocator(std::string_view name, SharedPtr<PMRResource> allocator);
		PMRResource* NewAllocator(std::string_view name);
	
	private:

		UnOrderedMap<std::string, SharedPtr<PMRResource>> m_AllocatorMap;
	};

}



