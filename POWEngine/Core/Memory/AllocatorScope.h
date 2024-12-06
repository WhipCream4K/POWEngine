#pragma once

namespace powe
{
    // The name of the allocator that will be registered to memory manager
    enum class AllocatorScope
    {
        Local,
        Application,
        Game,
        Logger,
        ThreadPool
    };

    inline std::string GetAllocatorName(AllocatorScope name)
    {
        switch (name)
        {
        case AllocatorScope::Application:
            return "Application";
        case AllocatorScope::Game:
            return "Game";
        case AllocatorScope::Logger:
            return "Logger";
        case AllocatorScope::ThreadPool:
            return "ThreadPool";
        default:
            return "";
        }
    }

}