#pragma once

#include <functional>

namespace powe
{
    class Scene;

    struct Input
    {
        enum State : uint8_t
        {
            Pressed,
            Released,
            Held
        };

        struct KeyBinding
        {
            int key;
            uint8_t modifiers;
            State state;
            std::function<void(Scene&)> callback;
            bool isValid;
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
        void AddKeyBinding(int key, Input::State state, std::function<void(Scene&)> callback, uint8_t modifiers = 0) noexcept;


        void Update() noexcept;
        const Vector<Input::KeyBinding>& GetKeyBindings() const noexcept { return m_KeyBindings; }
        Scene& GetScene() const noexcept { return *m_Scene; }

        // TODO: Also do the key binding detach

    private:

        RefWrap<Scene> m_Scene;
        Vector<Input::KeyBinding> m_KeyBindings;
        UniquePtr<InputSubsystem> m_InputSubsystem;
    };
}


