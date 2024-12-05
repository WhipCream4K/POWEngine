#pragma once

#include "AllocatorScope.h"

namespace powe
{

    class AllocatorContext
    {
    public:

        constexpr AllocatorContext(std::string_view allocatorName) noexcept
            : m_PreviosContext(m_CurrentContext)
        {
            m_CurrentContext = allocatorName.data();
        }

        constexpr AllocatorContext(AllocatorScope name) noexcept
            : m_PreviosContext(m_CurrentContext)
        {
            m_CurrentContext = GetAllocatorName(name);
        }

        ~AllocatorContext();

        // Only read operation so thread-safe
        std::pmr::memory_resource* GetResource() const noexcept;

    private:

        std::string m_PreviosContext;

        static thread_local std::string m_CurrentContext;
    };
}