#include "pch.h"

#include "OpenGLRenderContext.h"
#include "Platform/common/GLFW/glfwModule.h"
#include "Renderer/Viewport.h"

#include <GLFW/glfw3.h>

using namespace powe;

OpenGLRenderContext::OpenGLRenderContext()
{
    auto glfwDependency{Application::GetModule<glfwModule>()};
    if (!glfwDependency)
    {
        powe::Error("This version of openGL only supports GLFW for now and it cannot find GLFW module");
        throw std::runtime_error("GLFW module not found, cannot initialize OpenGL render context");
        return;
    }

    int version{gladLoadGLContext(&m_Context, glfwGetProcAddress)};

    if (version == 0)
    {
        powe::Error("Failed to initialize OpenGL render context");
        throw std::runtime_error("Failed to initialize OpenGL render context");
        return;
    }
    
    powe::Info("OpenGL render context initialized");

    // !! OpenGL leave the window's framebuffer presentation to the driver
    // so no render buffers will be generated for drawing !!

    // Pale Turquoise
    m_ClearColor = glm::fvec4(0.686f, 0.933f, 0.933f, 1.0f);

    // Culling and Depth test
    m_Context.Enable(GL_DEPTH_TEST);
    m_Context.DepthFunc(GL_LEQUAL);
    m_Context.Enable(GL_CULL_FACE);
    m_Context.FrontFace(GL_CCW);
    m_Context.CullFace(GL_BACK);

    // Set up blending
    m_Context.Enable(GL_BLEND);
    m_Context.BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set up primitive restart
    // Primitive restart is a mechanism that allows you to insert a special index value into your index buffer. 
    // When the GPU encounters this special value during rendering, it treats it as a signal to end the current primitive and start a new one, without the need for a new draw call.
    m_Context.Enable(GL_PRIMITIVE_RESTART);
    m_Context.PrimitiveRestartIndex(0xFFFF);
}

void OpenGLRenderContext::SetViewport(const Viewport& viewport) noexcept
{
    m_Context.Viewport(viewport.GetPosition().x, viewport.GetPosition().y
    , viewport.GetSize().x, viewport.GetSize().y);
}

void OpenGLRenderContext::BeginFrame()
{
    m_Context.ClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);
    m_Context.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderContext::SetClearColor(const glm::fvec4& color) noexcept
{
    m_ClearColor = color;
}

OpenGLRenderContext::~OpenGLRenderContext()
{
}