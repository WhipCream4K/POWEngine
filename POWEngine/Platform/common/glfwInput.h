#pragma once

#include "Game/Input/InputSubsystem.h"

// #include "glfwWindow.h"

class GLFWwindow;

namespace powe
{
    class Window;
    class glfwWindow;
    class glfwInput : public InputSubsystem
    {
    public:

        glfwInput(InputManager& inputManager);

        virtual ~glfwInput();

    private:

        WeakPtr<glfwWindow> m_BindWindow;
        GLFWwindow* m_WindowHandle;
    };
}