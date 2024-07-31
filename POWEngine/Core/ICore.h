#pragma once


namespace powe
{
    class Clock;
    class Renderer;
    class InputManager;
    class RenderAPI;
    class ICore
    {
    public:

        ICore();
        ~ICore();
        
        Clock& GetWorldClock() const {return *m_WorldClock;}
        InputManager& GetInputManager() const {return *m_InputManager;}
        
    protected:

        UniquePtr<Clock> m_WorldClock;
        UniquePtr<InputManager> m_InputManager;
    };
}

