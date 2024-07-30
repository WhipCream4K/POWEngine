#pragma once

#include <mutex>

namespace powe
{
	class MemoryTracker
	{
	public:

		void Allocate(void* ptr,size_t size);
		void DeAllocate(void* ptr);

		size_t GetTotalAllocateMemory() const {return m_TotalAllocateMemory;}

	private:

		std::unordered_map<void*, size_t> m_AllocatedMemoryMap;
		std::atomic_size_t m_TotalAllocateMemory{ 0 };
		std::mutex m_Mutex; // for thread safety
	};

}



