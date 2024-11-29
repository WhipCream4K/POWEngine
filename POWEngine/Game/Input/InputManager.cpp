#include "pch.h"
#include "InputManager.h"

#include "InputSubsystem.h"


using namespace powe;

InputManager::InputManager(Scene& scene)
    : m_Scene{scene}
{
    m_InputSubsystem = InputSubsystem::Create(*this);
}

InputManager::~InputManager() = default;

void InputManager::AddKeyBinding(int key, Input::State state, std::function<void(Scene&)> callback, uint8_t modifiers) noexcept
{
    m_KeyBindings.emplace_back(key, modifiers, state, callback, false);
}

void InputManager::Update() noexcept
{
    m_InputSubsystem->Update(m_KeyBindings);
}
