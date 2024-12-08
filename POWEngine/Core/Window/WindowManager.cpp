#include "pch.h"

#include "Core/Application/Application.h"
#include "WindowManager.h"
#include "Core/Window/Window.h"
#include "Logger/Logger.h"
#include "Core/Memory/AllocatorContext.h"

powe::WindowManager::WindowManager()
{
    const AllocatorContext context{};
    auto* upStream{context.GetResource()};
    m_ChildWindows = Vector<UniquePtr<Window>>{upStream};
}

powe::WindowManager::~WindowManager()
{
    // glfw Window Subsystem
    // glfwWindow::Shutdown();
}

void powe::WindowManager::Init()
{
    // glfw Window Subsystem
    // glfwWindow::Init();
}

void powe::WindowManager::Shutdown() noexcept
{
    // triggers subsystem dtor
    m_MainWindow.reset();
    m_ChildWindows.clear();
}

powe::Window* powe::WindowManager::CreateWindow(std::string_view windowName, uint32_t width, uint32_t height)
{
    // Using glfw module
    const AllocatorContext context{};
    auto* upStream{context.GetResource()};

    auto window{Window::Create(upStream, windowName, width, height)};
    
    if(!m_MainWindow)
    {
        m_MainWindow = std::move(window);

        return m_MainWindow.get();
    }

    const auto& last { m_ChildWindows.emplace_back(std::move(window)) };
    
    return last.get();
}

powe::Window* powe::WindowManager::GetWindow(std::string_view windowName) noexcept
{
    if(m_MainWindow && m_MainWindow->GetTitle() == windowName)
    {
        return m_MainWindow.get();
    }

    return std::ranges::find_if(m_ChildWindows, [windowName](const UniquePtr<Window>& window)
    {
        return window->GetTitle() == windowName;
    })->get();
}

void powe::WindowManager::DestroyWindow(std::string_view windowName)
{
    if(m_MainWindow && m_MainWindow->GetTitle() == windowName)
    {
        powe::Warning("Main window can only be deleted when calling application exit");
    }

    m_ChildWindows.erase(std::ranges::find_if(m_ChildWindows, [windowName](const UniquePtr<Window>& window)
    {
        return window->GetTitle() == windowName;
    }));
}

void powe::WindowManager::Update()
{
    // glfw Window Subsystem
    // glfwWindow::Update();
}