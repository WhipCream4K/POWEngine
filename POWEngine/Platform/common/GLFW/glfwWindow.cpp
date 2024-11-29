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

powe::glfwWindow::~glfwWindow()
{
    glfwDestroyWindow(m_WindowHandle);
    m_WindowHandle = nullptr;
}

// #endif
