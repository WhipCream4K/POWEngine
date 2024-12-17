#pragma once

namespace powe
{
    class IWindowObserver
    {
    public:

        IWindowObserver() = default;
        virtual ~IWindowObserver() = default;
    
        virtual void OnWindowResized(uint32_t width, uint32_t height) = 0;
        // add more window events if needed
    };
}