#pragma once
#include "Application.h"

namespace powe
{
    class AppResource
    {
    public:

        AppResource(const SharedPtr<Application>& application);

        Window* GetMainWindow() const;
        
    private:
    
        WeakPtr<Application> m_App;
    };
}


