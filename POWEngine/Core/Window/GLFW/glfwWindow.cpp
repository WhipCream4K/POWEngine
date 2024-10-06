#include "pch.h"

#include <Windows.h>

#ifdef USE_GLFW_WINDOW

#include <GLFW/glfw3.h>

#include "Core/Logger/LoggerUtils.h"
#include "Core/Window/Window.h"

powe::Window::Window(std::pmr::memory_resource* memResource, std::string_view title, int width, int height)
    : m_Title(title)
    , m_MemResource(memResource)
    , m_Width(width)
    , m_Height(height)
    , m_Fullscreen(false)
{
}

void powe::Window::Initialize()
{
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    GLFWwindow* window{ glfwCreateWindow( m_Width, m_Height, m_Title.c_str(), glfwGetPrimaryMonitor(), nullptr) };
    std::pmr::polymorphic_allocator<SharedPtr<GLFWwindow>> alloc(m_MemResource);
    
    m_WindowHandle = std::allocate_shared<SharedPtr<GLFWwindow>>(alloc, window, [this](GLFWwindow* window)
    {
        glfwDestroyWindow(window);
    });

    glfwSetWindowUserPointer(window,this);

    glfwSetWindowFocusCallback(window, [](GLFWwindow* window, int focused)
    {
        Window* win{ static_cast<Window*>(glfwGetWindowUserPointer(window)) };
        win->m_IsFocused = bool(focused);
    });
}

bool powe::Window::PollEvents(EventQueue& eventQueue)
{
    glfwPollEvents();
    eventQueue = {this,{}};
    return ShouldClose();
}

void powe::Window::SetFullscreen(bool fullscreen, bool borderless)
{
}

void powe::Window::SetTitle(const std::string& title)
{
}

void powe::Window::Resize(int width, int height)
{
}

bool powe::Window::IsFocused() const
{
    return m_IsFocused;
}

bool powe::Window::ShouldClose() const
{
    return glfwWindowShouldClose(GetWindowHandle<GLFWwindow>());
}

void powe::Window::InitializeLibrary()
{
    glfwInitHint(GLFW_JOYSTICK_HAT_BUTTONS, GLFW_TRUE);
    if (!glfwInit())
    {
        const char* description;
        glfwGetError(&description);
        if(description)
        {
            POWE_LOGERROR("Failed to initialize GLFW: %s", description);
        }
        throw std::runtime_error("Failed to initialize GLFW");
    }
    
}

void powe::Window::TerminateLibrary()
{
    glfwTerminate();
}

#endif
