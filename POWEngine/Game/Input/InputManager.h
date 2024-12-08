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
    class Game;
    class InputManager
    {
    public:

        InputManager(Game& gameModule);
        InputManager(const InputManager&) = delete;
        InputManager& operator=(const InputManager&) = delete;
        InputManager(InputManager&&) = default;
        InputManager& operator=(InputManager&&) = default;
        ~InputManager() = default;

        Game& GetGameModule() const noexcept { return *m_GameModule; }

        /// @brief Registers a key binding
        /// @param key The key to bind normally uses GLFW keys value
        /// @param state The state of the key
        /// @param callback The function to call
        /// @return
        void AddKeyBinding(int key, Input::State state, std::function<void(Scene&)> callback, uint8_t modifiers = 0) noexcept;


        void Update() noexcept;
        const Vector<Input::KeyBinding>& GetKeyBindings() const noexcept { return m_KeyBindings; }

        // TODO: Also do the key binding detach

    private:

        RefWrap<Game> m_GameModule;
        Vector<Input::KeyBinding> m_KeyBindings;
        UniquePtr<InputSubsystem> m_InputSubsystem;
    };
}


