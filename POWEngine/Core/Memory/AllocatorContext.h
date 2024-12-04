#pragma once

#include "AllocatorScope.h"

namespace powe
{

    class AllocatorContext
    {
    public:

        constexpr AllocatorContext(std::string_view allocatorName) noexcept;
        constexpr AllocatorContext(AllocatorScope name) noexcept;

        ~AllocatorContext();

        // Only read operation so thread-safe
        std::pmr::memory_resource* GetResource() const noexcept;

    private:

        std::string m_PreviosContext;

        static thread_local std::string m_CurrentContext;
    };
}