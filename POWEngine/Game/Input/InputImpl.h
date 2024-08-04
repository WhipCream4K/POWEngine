#pragma once
#include "Core/WindowManager.h"

namespace powe
{
    template<typename T>
    class InputImpl
    {
    public:
        InputImpl() = default;
        InputImpl(const InputImpl&) = delete;
        InputImpl& operator=(const InputImpl&) = delete;
        InputImpl(InputImpl&&) = delete;
        InputImpl& operator=(InputImpl&&) = delete;
        virtual ~InputImpl() = default;

        
        virtual void OnWindowEvents(WindowManager::Events& events) = 0;
    };
}

