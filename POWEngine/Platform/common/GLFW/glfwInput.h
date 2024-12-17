#pragma once

#include "Game/Input/InputSubsystem.h"


struct GLFWwindow;

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

        GLFWwindow* m_WindowHandle;
    };
}