#include "pch.h"
#include "WindowManager.h"
#include "Utils/Utils.h"

powe::WindowManager::WindowManager(PMRResource* memResource)
    : m_Windows{memResource}
      , m_MainWindow(nullptr)
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

powe::WindowManager::Events powe::WindowManager::Update()
{
    std::pmr::monotonic_buffer_resource eventBufferResource(1024);
    Events events{m_MemResource};
    
    for (auto& window : m_Windows)
    {
        window.PollEvents(events.eventQueue);
        events.window = &window;
    }
    return events;
}
