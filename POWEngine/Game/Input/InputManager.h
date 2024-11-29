#pragma once

#include <functional>

namespace powe
{
    class Scene;

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
            std::function<void(Scene&)> callback;
        };
    };
    
    class InputSubsystem;
    class InputManager
    {
    public:

        InputManager(Scene& scene);
        ~InputManager();

        /// @brief Registers a key binding
        /// @param key The key to bind normally uses GLFW keys value
        /// @param state The state of the key
        /// @param callback The function to call
        /// @return
        void AddKeyBinding(int key, Input::State state, std::function<void(Scene&)> callback, int modifiers = 0) noexcept;


    private:

        RefWrap<Scene> m_Scene;
        Vector<Input::KeyBinding> m_KeyBindings;
        UniquePtr<InputSubsystem> m_InputSubsystem;
    };
}


