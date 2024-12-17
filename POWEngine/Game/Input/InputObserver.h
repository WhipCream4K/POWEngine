#pragma once 

#include "InputManager.h"

namespace powe
{
    class InputObserver : public std::enable_shared_from_this<InputObserver>
    {
    public:
    
        InputObserver() = default;
        virtual ~InputObserver();

        virtual void Execute(Scene& scene) noexcept = 0;
    };
}