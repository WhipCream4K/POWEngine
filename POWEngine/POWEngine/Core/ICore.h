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

        OwnedPtr<WorldClock> m_WorldClock;
        OwnedPtr<InputManager> m_InputManager;
    };
}

