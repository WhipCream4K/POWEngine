#pragma once


namespace powe
{
    class WorldClock;
    class Renderer;
    class InputManager;
    class RenderAPI;
    class ICore
    {
    public:

        ICore();
        ~ICore();
        
        WorldClock& GetWorldClock() const {return *m_WorldClock;}
        InputManager& GetInputManager() const {return *m_InputManager;}
        
    protected:

        UniquePtr<WorldClock> m_WorldClock;
        UniquePtr<InputManager> m_InputManager;
    };
}

