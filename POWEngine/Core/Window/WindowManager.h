#pragma once

#include "Window.h"

namespace powe
{
    class WindowEvent;
    class WindowSubsystem;
    class WindowManager final
    {
    public:
        
        WindowManager();
        WindowManager(const WindowManager&) = delete;
        WindowManager& operator=(const WindowManager&) = delete;
        WindowManager(WindowManager&&) noexcept = default;
        WindowManager& operator=(WindowManager&&) noexcept = default;
        ~WindowManager();

        void Init();
        void Shutdown() noexcept;
    
        Window* CreateWindow(std::string_view windowName, uint32_t width, uint32_t height);
        Window* GetWindow(std::string_view windowName) noexcept;
        Window* GetMainWindow() noexcept { return m_MainWindow.get(); }
        void DestroyWindow(std::string_view windowName);
        void Update();

    private:

        SharedPtr<PMRResource> GetResource() const noexcept;
    
        UniquePtr<Window> m_MainWindow;
        Vector<UniquePtr<Window>> m_ChildWindows;
    };
}
