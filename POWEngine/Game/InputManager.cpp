#include "pch.h"
#include "InputManager.h"

#include "GLFW/glfw3.h"

using namespace powe;

InputManager::InputManager(const SharedPtr<PMRResource>& memResource)
    : m_DefaultResource(memResource)
{
    
}
