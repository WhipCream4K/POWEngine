#include "pch.h"
#include "glfwWindow.h"

#include "glfwWindowViewport.h"

#include "Platform/common/GL/OpenGLModule.h"

#include <GLFW/glfw3.h>

using namespace powe;

powe::glfwWindow::glfwWindow(std::string_view title, int width, int height)
    : m_Title(title)
    , m_Width(width)
    , m_Height(height)
{
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

    // check if opengl exist
    auto openGLModule{ Application::GetModule<OpenGLModule>() };
    if(!openGLModule)
    {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);      
    }
    else 
    {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    }

    
    m_WindowHandle = glfwCreateWindow( width, height, m_Title.c_str(), glfwGetPrimaryMonitor(), nullptr);

    glfwSetWindowUserPointer(m_WindowHandle,this);
    
    AllocatorContext context{};
    auto* upStream{context.GetResource()};

    m_Viewport = AllocateShared<glfwWindowViewport>(
        upStream, *this, 
    glm::uvec2{0u,0u}, 
    glm::uvec2{m_Width, m_Height});

}

powe::glfwWindow::~glfwWindow()
{
    glfwDestroyWindow(m_WindowHandle);
    m_WindowHandle = nullptr;
}

void powe::glfwWindow::OnCreate(WindowManager *windowManager) noexcept
{
    m_WindowManager = windowManager;
    
    glfwSetWindowSizeCallback(m_WindowHandle,[](GLFWwindow* window, int width, int height)
    {
        auto* self = static_cast<glfwWindow*>(glfwGetWindowUserPointer(window));
        self->ResizeCallback(width, height);
        self->m_WindowManager->NotifyObserverOfResize(width, height);
    });

}


void powe::glfwWindow::SetTitle(std::string_view title) noexcept
{
    m_Title = title;

    if(IsInAppMainThread())
    {
        glfwSetWindowTitle(m_WindowHandle, m_Title.c_str());
    }

}

void powe::glfwWindow::ResizeCallback(uint32_t width, uint32_t height) noexcept
{
    m_Width = width;
    m_Height = height;
}

void powe::glfwWindow::Resize(uint32_t width, uint32_t height) noexcept
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

bool powe::glfwWindow::IsFullscreen() const noexcept
{
    return glfwGetWindowMonitor(m_WindowHandle) != nullptr;
}

bool glfwWindow::IsFocused() const noexcept
{
    return glfwGetWindowAttrib(m_WindowHandle, GLFW_FOCUSED);
}

void powe::glfwWindow::SetFullscreen(bool fullscreen, [[maybe_unused]] bool borderless) noexcept
{
    if(fullscreen)
    {
        auto monitor{ glfwGetWindowMonitor(m_WindowHandle) };
        auto videoMode{ glfwGetVideoMode(monitor) };

        glfwSetWindowMonitor(m_WindowHandle, monitor, 0, 0, videoMode->width, videoMode->height, GLFW_DONT_CARE);
    }
    else {
        glfwSetWindowMonitor(m_WindowHandle, nullptr, 0, 0, m_Width, m_Height, GLFW_DONT_CARE);
    }
}

// #endif
