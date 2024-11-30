#include "pch.h"

#include "glfwInput.h"
#include "Game/Game.h"
#include "Game/Scene.h"
#include "glfwWindow.h"


#include <execution>
#include <GLFW/glfw3.h>


using namespace powe;

std::function<void(GLFWwindow*,int,int,int,int)> KeyCallback{};

static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    KeyCallback(window,key, scancode, action, mods);
}

glfwInput::glfwInput(InputManager &inputManager) 
    : InputSubsystem(inputManager)
{
    auto bindWindow{inputManager.GetScene().GetGameModule().GetBindWindow()};
    if (auto glfwWindowPtr{dynamic_cast<glfwWindow*>(bindWindow)}; glfwWindowPtr)
    {
        m_WindowHandle = glfwWindowPtr->GetHandle();

        KeyCallback = [this](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            if(window == m_WindowHandle)
            {
                auto& keyBindinds{m_InputManager->GetKeyBindings()};

                // not sure if it can be call unsequential and parallel then
                for(const auto& keyBinding : keyBindinds)
                {
                    if (keyBinding.key == key && keyBinding.state == action && keyBinding.modifiers == mods)
                    {
                        keyBinding.callback(m_InputManager->GetScene());
                        return;
                    }
                }

                // std::for_each(std::execution::par_unseq, keyBindinds.begin(), keyBindinds.end(), 
                // [this,key, action,mods](Input::KeyBinding &keyBinding)
                // {
                //     if (keyBinding.key == key && keyBinding.state == action && keyBinding.modifiers == mods)
                //     {
                //         keyBinding.callback(m_InputManager->GetScene());
                //     }      
                // });
            }

        };

        glfwSetKeyCallback(m_WindowHandle, glfwKeyCallback);
    }
}

glfwInput::~glfwInput()
{
    glfwSetKeyCallback(m_WindowHandle, nullptr);
}