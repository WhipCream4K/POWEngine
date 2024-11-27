#include "pch.h"
#include "glfwWindow.h"

#include "Utils/Utils.h"

powe::glfwWindow::glfwWindow(std::string_view title, int width, int height)
    : m_Title(title)
    , m_Width(width)
    , m_Height(height)
{
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    
    m_WindowHandle = glfwCreateWindow( width, height, m_Title.c_str(), glfwGetPrimaryMonitor(), nullptr);

    glfwSetWindowUserPointer(m_WindowHandle,this);
}


void powe::glfwWindow::SetTitle(std::string_view title)
{
    m_Title = title;

    if(IsInAppMainThread())
    {
        glfwSetWindowTitle(m_WindowHandle, m_Title.c_str());
    }

}

void powe::glfwWindow::Resize(uint32_t width, uint32_t height) 
{
    m_Width = width;
    m_Height = height;
    
    if(IsInAppMainThread())
    {
        glfwSetWindowSize(m_WindowHandle, width, height);
    }
}

bool powe::glfwWindow::IsClosed() const noexcept
{
    return glfwWindowShouldClose(m_WindowHandle);
}

void powe::glfwWindow::Init()
{
    glfwInitHint(GLFW_JOYSTICK_HAT_BUTTONS, GLFW_TRUE);
    if (!glfwInit())
    {
        const char* description;
        glfwGetError(&description);
        if(description)
        {
            // TODO: Add logging system
            // POWE_LOGERROR(std::string("Failed to initialize GLFW: ") + description);
        }
        throw std::runtime_error("Failed to initialize GLFW");
    }
}

void powe::glfwWindow::Update()
{
    glfwPollEvents();
}

void powe::glfwWindow::Shutdown()
{
    glfwTerminate();
}

powe::glfwWindow::~glfwWindow()
{
    glfwDestroyWindow(m_WindowHandle);
}

// #endif
