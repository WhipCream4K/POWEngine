#pragma once

#include "Memory/Allocator.h"
#include "Window/Window.h"

namespace powe
{
    class WindowManager final
    {
    public:
        
        WindowManager(PMRResource* memResource = DefaultAllocator::Application);
        WindowManager(const WindowManager&) = delete;
        WindowManager& operator=(const WindowManager&) = delete;
        WindowManager(WindowManager&&) noexcept = delete;
        WindowManager& operator=(WindowManager&&) noexcept = delete;
        ~WindowManager();
        
        Window& CreateWindow(std::string_view title, int width, int height);
        Window& GetMainWindow() { return m_Windows.front(); }
        const Vector<Window>& GetWindows() { return m_Windows; }
        Window& GetWindow(std::string_view windowName);
        const Window::EventQueue& GetFocusedEvent() const {return m_CurrentWindowEvents;}
        void DestroyWindow(std::string_view windowName);
        

        Window::EventQueue Update();

    private:
        
        Vector<Window> m_Windows;
        
        // From testing polling window events, 1024 bytes is enough to store all events
        // of windows happening in a single frame
        std::array<std::byte,1024> m_EventBuffer;
        Window::EventQueue m_CurrentWindowEvents;
        
        Window* m_ActiveWindow;
        PMRResource* m_MemResource;
    };
}
