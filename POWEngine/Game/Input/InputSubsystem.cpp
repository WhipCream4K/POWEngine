#include "pch.h"
#include "InputSubsystem.h"

#include "Game/Scene.h"
#include "Game/Game.h"
#include "Utils/Utils.h"
#include "Core/Memory/AllocatorContext.h"

#include "Platform/common/GLFW/glfwInput.h"

using namespace powe;

UniquePtr<InputSubsystem> InputSubsystem::Create(InputManager& inputManager)
{
    const AllocatorContext context{AllocatorScope::Game};
    auto* upStream{context.GetResource()};
    return AllocateUnique<glfwInput>(upStream,inputManager);
}