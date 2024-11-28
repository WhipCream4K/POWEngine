#pragma once

#include <functional>

namespace powe
{
    // struct Input
    // {
    //     enum State
    //     {
    //         Pressed,
    //         Released,
    //         Held
    //     };

    //     struct KeyBinding
    //     {
    //         int key;
    //         State state;
    //         std::function<void()> callback;
    //     };
    // };
    
    class InputManager
    {
    public:

        InputManager(const SharedPtr<PMRResource>& memResource);
        

    // private:
        
        // Vector<Input::KeyBinding> m_KeyBindings;
    };

    // template <typename T> requires std::is_invocable_v<T>
    // void InputManager::AddKeyBinding(int key, Input::State state, T&& func)
    // {
    //     m_KeyBindings.emplace_back({key,state,std::forward<T>(func)});
    // }
}


