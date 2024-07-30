#include "pch.h"
#include "MemoryTracker.h"

void powe::MemoryTracker::Allocate(void* ptr, size_t size)
{
	{
		std::scoped_lock lock(m_Mutex);
		m_AllocatedMemoryMap.try_emplace(ptr, size);
	}

	m_TotalAllocateMemory.fetch_add(size);
}

void powe::MemoryTracker::DeAllocate(void* ptr)
{
	size_t memorySize{};

	{
		std::scoped_lock lock(m_Mutex);
		auto it = m_AllocatedMemoryMap.find(ptr);
		if (it != m_AllocatedMemoryMap.end())
		{
			memorySize = it->second;
			m_AllocatedMemoryMap.erase(it);
		}
	}

	m_TotalAllocateMemory.fetch_sub(memorySize);
}

