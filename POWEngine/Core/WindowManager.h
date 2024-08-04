#pragma once

#include "Window/Window.h"
#include "Core/CustomTypes.h"

namespace powe
{
    class WindowManager final
    {
    public:
        struct Events
        {
            Events(PMRResource* memResource)
                : eventQueue(memResource), window(nullptr)
            {
            }

            Window::EventQueue eventQueue;
            Window* window;
        };

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

        Events Update();

    private:
        Vector<Window> m_Windows;
        Window* m_MainWindow;
        PMRResource* m_MemResource;
    };
}
