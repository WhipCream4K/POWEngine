#include "pch.h"
#include "glfwModule.h"
#include "Core/Application/Application.h"

#include "Logger/Logger.h"
#include "GLFW/glfw3.h"

using namespace powe;

glfwModule::glfwModule()
    : IModule("GLFW")
{
    glfwInitHint(GLFW_JOYSTICK_HAT_BUTTONS, GLFW_TRUE);
    if (!glfwInit())
    {
        const char* description;
        glfwGetError(&description);
        if(description)
        {
            powe::Info(std::string("Failed to initialize GLFW: ") + description);
        }
        throw std::runtime_error("Failed to initialize GLFW");
    }
}

void glfwModule::OnCreate(ModulesManager* modulesManager)
{
    auto appResource{ modulesManager->GetModuleResource<glfwModule>() };
    m_GlfwEvent = std::allocate_shared<glfwEvent>(appResource);
    
    Application::Get().RegisterAppEvent(m_GlfwEvent);
}

void glfwModule::OnExit(ModulesManager* modulesManager)
{
    glfwTerminate();
}

void glfwEvent::OnUpdate(float)
{
    glfwPollEvents();
}