#include "pch.h"
#include "AllocatorContext.h"

#include "MemoryManager.h"

using namespace powe;

std::string thread_local powe::AllocatorContext::m_CurrentContext{"Application"};

std::pmr::memory_resource* powe::AllocatorContext::GetResource() const noexcept
{
    return MemoryManager::Get()->GetAllocator(m_CurrentContext);
}

constexpr AllocatorContext::AllocatorContext(std::string_view allocatorName) noexcept
{
    m_PreviosContext = m_CurrentContext;
    m_CurrentContext = allocatorName;
}

constexpr AllocatorContext::AllocatorContext(AllocatorScope name) noexcept
{
    m_PreviosContext = m_CurrentContext;
    m_CurrentContext = GetAllocatorName(name);
}


AllocatorContext::~AllocatorContext()
{
    m_CurrentContext = m_PreviosContext;
}