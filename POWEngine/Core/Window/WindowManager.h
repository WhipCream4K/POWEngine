#pragma once

#include <functional>

#include "Core/Memory/Allocator.h"
#include "Window.h"
namespace powe
{
    class WindowEvent;
    class WindowSubsystem;
    class WindowManager final
    {
    public:
        
        WindowManager(const SharedPtr<PMRResource>& memResource);
        WindowManager(const WindowManager&) = delete;
        WindowManager& operator=(const WindowManager&) = delete;
        WindowManager(WindowManager&&) noexcept = default;
        WindowManager& operator=(WindowManager&&) noexcept = default;
        ~WindowManager();

        void Init();
        void Shutdown() noexcept;

    
        SharedPtr<Window> CreateWindow(std::string_view windowName, uint32_t width, uint32_t height);
        SharedPtr<Window> GetWindow(std::string_view windowName) noexcept;
        SharedPtr<Window> GetMainWindow() noexcept { return m_MainWindow; }
        void DestroyWindow(std::string_view windowName);
        void Update();

    private:

        SharedPtr<PMRResource> GetResource() const noexcept;
    
        SharedPtr<Window> m_MainWindow;
        Vector<SharedPtr<Window>> m_ChildWindows;
        SharedPtr<PMRResource> m_DefaultRescource;
    };
}
