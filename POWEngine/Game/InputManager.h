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

        struct KeyBinding
        {
            int key;
            State state;
            std::function<void()> callback;
        };
    };
    
    class InputManager
    {
    public:

        InputManager(PMRResource* memResource)
            : m_KeyBindings(memResource)
        {
        }

        /**
         * @brief This function is called by the scene to handle the input events
         * using glfw to get the state of the keys
         * @param events injection events from window manager
         */
        void OnWindowEvents(const Window::EventQueue& events);

        template<typename T> requires std::is_invocable_v<T>
        void AddKeyBinding(int key, Input::State state,T&& func);
        
    private:
        
        Vector<Input::KeyBinding> m_KeyBindings;
    };

    template <typename T> requires std::is_invocable_v<T>
    void InputManager::AddKeyBinding(int key, Input::State state, T&& func)
    {
        m_KeyBindings.emplace_back({key,state,std::forward<T>(func)});
    }
}


