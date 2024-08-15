#include "pch.h"
#include "WindowManager.h"

#include "Application/Application.h"

powe::WindowManager::WindowManager(PMRResource* memResource)
    : m_Windows{memResource}
      , m_ActiveWindow()
      , m_MemResource(memResource)
{
    Window::InitializeLibrary();
}

powe::WindowManager::~WindowManager()
{
    Window::TerminateLibrary();
}

powe::Window& powe::WindowManager::CreateWindow(std::string_view title, int width, int height)
{
    return m_Windows.emplace_back(m_MemResource, title, width, height);
}

powe::Window& powe::WindowManager::GetWindow(std::string_view windowName)
{
    return *std::ranges::find_if(m_Windows, [windowName](const Window& window)
    {
        return window.GetTitle() == windowName;
    });
}

void powe::WindowManager::DestroyWindow(std::string_view windowName)
{
    std::erase_if(m_Windows, [windowName](const Window& window)
    {
        return window.GetTitle() == windowName;
    });
}

powe::Window::EventQueue powe::WindowManager::Update()
{
    std::array<std::byte, 1024> buffer{};
    std::pmr::monotonic_buffer_resource memResource(buffer.data(), buffer.size());

    Window::EventQueue windowEvents{};

    for (auto& window : m_Windows)
    {
        if (window.IsFocused())
        {
            m_ActiveWindow = &window;
            window.PollEvents(windowEvents);
            break;
        }
    }
    return windowEvents;
}
