#include "pch.h"
#include "Window.h"

#include "Utils/Utils.h"
#include "Platform/common/GLFW/glfwWindow.h"

using namespace powe;


UniquePtr<Window> Window::Create(PMRResource* allocator,std::string_view windowName, uint32_t width, uint32_t height)
{   
    return AllocateUnique<glfwWindow>(allocator,windowName, width, height);
}