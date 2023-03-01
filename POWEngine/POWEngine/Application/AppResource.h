#pragma once
#include "POWEngine/Window/Window.h"

namespace powe
{
    class AppResource
    {
    public:

        Window* GetGameWindow() const {return m_GameWindow.get();}
        
    protected:
        
        SharedPtr<Window> m_GameWindow;
    };
}

