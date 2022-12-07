#include "pch.h"
#include "AppResource.h"

powe::AppResource::AppResource(const SharedPtr<Application>& application)
    : m_App(application)
{
}

powe::Window* powe::AppResource::GetMainWindow() const
{
    if(const SharedPtr<Application> appReal{m_App.lock()})
    {
        return appReal->GetGameWindow();
    }

    return nullptr;
}
