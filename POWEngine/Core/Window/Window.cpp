#include "pch.h"
#include "Window.h"

#include "Utils/Utils.h"
#include "Platform/common/GLFW/glfwWindow.h"

using namespace powe;


UniquePtr<Window> Window::Create(const SharedPtr<PMRResource>& resc,std::string_view windowName, uint32_t width, uint32_t height)
{   
    return AllocateUnique<glfwWindow>(resc,windowName, width, height);
}