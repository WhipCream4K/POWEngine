#include "pch.h"
#include "ICore.h"

#include "Clock/WorldClock.h"
#include "Input/InputManager.h"
#include "POWEngine/Renderer/NullRenderer.h"
#include "POWEngine/Renderer/Renderer.h"

powe::ICore::ICore()
    : m_WorldClock(std::make_unique<WorldClock>())
    , m_InputManager(std::make_unique<InputManager>())
{
}

powe::ICore::~ICore() = default;