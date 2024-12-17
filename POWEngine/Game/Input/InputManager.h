#pragma once


namespace powe
{
    class Scene;
    class InputObserver;

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

            bool operator==(const KeyBinding& rhs) const noexcept { 
                return key == rhs.key && modifiers == rhs.modifiers && state == rhs.state; 
            }
        };

        using KeyBindingObservers = Vector<std::pair<Input::KeyBinding, Vector<WeakPtr<InputObserver>>>>;
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
        /// @param observer
        /// @param key The key to bind normally uses GLFW keys value
        /// @param state The state of the key
        /// @param callback The function to call
        /// @return
        void AddKeyBinding(const SharedPtr<InputObserver>& observer,int key, Input::State state, uint8_t modifiers = 0) noexcept;
        void RemoveObserver(const SharedPtr<InputObserver>& observer) noexcept;
        const Vector<WeakPtr<InputObserver>>* GetObservers(const Input::KeyBinding& binding) const noexcept;
        
        void Update() noexcept;

    private:

        RefWrap<Game> m_GameModule;
        Input::KeyBindingObservers m_Observers;
        UniquePtr<InputSubsystem> m_InputSubsystem;
    };
}


