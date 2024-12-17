#include "pch.h"

#include "OpenGLRenderContext.h"
#include "Platform/common/GLFW/glfwModule.h"
#include "Core/Memory/AllocatorContext.h"
#include "Renderer/Viewport.h"

#include <GLFW/glfw3.h>

using namespace powe;

OpenGLRenderContext::OpenGLRenderContext(uint32_t renderBufferCount)
{

    auto glfwDependency{Application::GetModule<glfwModule>()};
    if (!glfwDependency)
    {
        powe::Error("GLFW module not found, cannot initialize OpenGL render context");
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

    // Allocate render buffers
    AllocatorContext allocContext{};
    auto* allocator{allocContext.GetResource()};
    m_RenderBuffers = Vector<GLuint>{allocator};
    m_RenderBuffers.resize(renderBufferCount, 0);
    m_Context.GenRenderbuffers(renderBufferCount, m_RenderBuffers.data());
    m_ActiveRenderBuffer = 0;

    // m_Context.RenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 800, 600);

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
    // From Claude 3.5 Sonnet: What is Primitive Restart? Primitive restart is a mechanism that allows you to insert a special index value into your index buffer. 
    // When the GPU encounters this special value during rendering, it treats it as a signal to end the current primitive and start a new one, without the need for a new draw call.
    m_Context.Enable(GL_PRIMITIVE_RESTART);
    m_Context.PrimitiveRestartIndex(0xFFFF);
}

void OpenGLRenderContext::SetViewport(const Viewport& viewport)
{
    m_Context.Viewport(viewport.GetPosition().x, viewport.GetPosition().y
    , viewport.GetSize().x, viewport.GetSize().y);
}

OpenGLRenderContext::~OpenGLRenderContext()
{
    m_Context.DeleteFramebuffers(m_RenderBuffers.size(), m_RenderBuffers.data());
    m_RenderBuffers.clear();
}