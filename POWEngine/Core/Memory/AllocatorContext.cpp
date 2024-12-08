#include "pch.h"
#include "AllocatorContext.h"

#include "MemoryManager.h"

using namespace powe;

std::string thread_local powe::AllocatorContext::m_CurrentContext{"Application"};

std::pmr::memory_resource* powe::AllocatorContext::GetResource() const noexcept
{
    return MemoryManager::Get()->GetAllocator(m_CurrentContext);
}