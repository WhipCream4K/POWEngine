#include "pch.h"
#include "InputSubsystem.h"

#include "Game/Scene.h"
#include "Game/Game.h"
#include "Utils/Utils.h"

#include "Platform/common/GLFW/glfwInput.h"

using namespace powe;

UniquePtr<InputSubsystem> InputSubsystem::Create(InputManager& inputManager)
{
    return AllocateUnique<glfwInput>(inputManager.GetScene().GetResource(),inputManager);
}