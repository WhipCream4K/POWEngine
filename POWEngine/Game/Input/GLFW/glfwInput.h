#pragma once

#include "Game/Input/InputImpl.h"
#include <functional>

namespace powe
{
    class glfwInput : public InputImpl
    {
    public:

        using KeyBinding = std::pair<int, std::function<void()>>;

        glfwInput(PMRResource* memResource)
            : m_KeyBindings(memResource)
        {
        }

        void OnWindowEvents(WindowManager::Events& events) override;
        
    private:

        Vector<KeyBinding> m_KeyBindings;
    };
}

