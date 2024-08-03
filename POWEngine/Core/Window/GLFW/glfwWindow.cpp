#include "pch.h"

#ifdef USE_GLFW_WINDOW

#include <GLFW/glfw3.h>

#include "Core/Logger/LoggerUtils.h"
#include "Core/Window/Window.h"

powe::Window::Window(std::pmr::memory_resource* memResource, std::string_view title, int width, int height)
    : m_MemResource(memResource)
    , m_Width(width)
    , m_Height(height)
    , m_Fullscreen(false)
{
}

powe::Window::~Window()
{
    
}

void powe::Window::Initialize()
{
    
}

void powe::Window::SetFullscreen(bool)
{
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

void powe::Window::CreateWindow()
{
    
}

void powe::Window::DestroyWindow()
{
    
}

#endif
