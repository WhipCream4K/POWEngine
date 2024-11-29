#pragma once

#include "InputManager.h"

namespace powe
{
    class InputSubsystem
    {
    public:

        InputSubsystem(InputManager& inputManager) : m_InputManager(inputManager) {}
    
        virtual ~InputSubsystem() = default;

        virtual void Update(Vector<Input::KeyBinding>& keyBindings) noexcept {}

    protected:

        RefWrap<InputManager> m_InputManager;
    };
}