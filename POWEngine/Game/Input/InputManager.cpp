#include "pch.h"
#include "InputManager.h"

#include "GLFW/glfw3.h"

using namespace powe;

InputManager::InputManager(Scene& scene)
    : m_Scene{scene}
{
    
}

InputManager::~InputManager() = default;
