#include "pch.h"
#include "Windowmanager.h"
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

powe::Window* powe::WindowManager::CreateWindow(std::string_view title, int width, int height)
{
    auto window = AllocateUnique<Window>(m_MemResource, title, width, height);
    window->Initialize();
    
    m_Windows.try_emplace(title.data(),std::move(window));

    // Set main window for the first window created
    if (!m_MainWindow)
    {
        m_MainWindow = m_Windows[title.data()].get();
    }
    
    return m_Windows[title.data()].get();
}

void powe::WindowManager::DestroyWindow(std::string_view windowName)
{
    if (auto it = m_Windows.find(windowName.data()); it != m_Windows.end())
    {
        m_Windows.erase(it);
    }
}
