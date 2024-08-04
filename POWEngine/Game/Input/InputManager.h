#pragma once
#include "Core/WindowManager.h"
#include <functional>
#include <variant>

namespace powe
{

    struct Input
    {
        enum State
        {
            Pressed,
            Released,
            Held
        };
    };

    template<typename T>
    concept CInputImpl = requires(T t)
    {
        { t.OnWindowEvents(std::declval<WindowManager::Events&>()) } -> std::same_as<void>;
        { t.AddKeyBinding(std::declval<int>(),
            std::declval<Input::State>() ,std::declval<std::function<void>>()) } -> std::same_as<void>;
    };

    template<CInputImpl InputImpl>
    class InputManager
    {
    public:

        InputManager(PMRResource* memResource)
        {
        }

        void OnWindowEvents(WindowManager::Events& events);

        template<typename T> requires std::is_invocable_v<T>
        void AddKeyBinding(int key, Input::State state,T&& func);
        
    private:
        
        InputImpl m_InputImpl;
    };
}


