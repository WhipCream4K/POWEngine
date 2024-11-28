#include "pch.h"
#include "InputManager.h"


using namespace powe;

InputManager::InputManager(const SharedPtr<PMRResource>& memResource)
    : m_DefaultResource(memResource)
{
}
