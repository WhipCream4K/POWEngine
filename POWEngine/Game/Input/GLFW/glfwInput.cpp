#include "pch.h"
#include "glfwInput.h"

#include <GLFW/glfw3.h>

void powe::glfwInput::OnWindowEvents(WindowManager::Events& events)
{
    for (auto& [keyCode,callback] : m_KeyBindings)
    {
        const int scanCode = glfwGetKeyScancode(keyCode);
        
    }
}
