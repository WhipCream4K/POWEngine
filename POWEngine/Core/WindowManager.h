#pragma once

#include "Window/Window.h"

namespace powe
{
    class WindowManager final
    {
    public:
        
        WindowManager(PMRResource* memResource);
        WindowManager(const WindowManager&) = delete;
        WindowManager& operator=(const WindowManager&) = delete;
        WindowManager(WindowManager&&) = delete;
        WindowManager& operator=(WindowManager&&) = delete;
        ~WindowManager();
        
        Window& CreateWindow(std::string_view title, int width, int height);
        Window& GetMainWindow() { return m_Windows.front(); }
        Window& GetWindow(std::string_view windowName);
        void DestroyWindow(std::string_view windowName);
        

        Window::EventQueue Update();

    private:
        Vector<Window> m_Windows;
        Window* m_ActiveWindow;
        PMRResource* m_MemResource;
    };
}
