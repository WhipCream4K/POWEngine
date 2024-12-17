#include "pch.h"

#include "glfwWindowViewport.h"
#include "glfwWindow.h"

#include <GLFW/glfw3.h>

using namespace powe;

void glfwWindowViewport::Bind(RenderContext &)
{
    glfwMakeContextCurrent(m_WindowHandle->GetHandle());
}

void glfwWindowViewport::Present(RenderContext &)
{
    glfwSwapBuffers(m_WindowHandle->GetHandle());
}