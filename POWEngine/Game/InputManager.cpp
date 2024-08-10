#include "pch.h"
#include "InputManager.h"

#include "Core/Window/Window.h"

#include <GLFW/glfw3.h>

void powe::InputManager::OnWindowEvents(const Window::EventQueue& events)
{
    const auto& window = events.first;

    for (auto& [key,userTargetState,callback] : m_KeyBindings)
    {
        const int state{glfwGetKey(window->GetWindowHandle<GLFWwindow>(), key)};
        if (state == userTargetState)
        {
            callback();
        }
    }
}
