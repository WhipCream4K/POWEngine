#include "pch.h"
#include "SingleInstanceContainer.h"

SharedPtr<powe::Core> powe::SingleInstanceContainer::GetCore() const
{
    if(SharedPtr<Core> realCore{m_Core.lock()})
        return realCore;

    return nullptr;
}

SharedPtr<powe::Application> powe::SingleInstanceContainer::GetSharedApplication() const
{
    if(SharedPtr<Application> realApp{m_Application.lock()})
        return realApp;

    return nullptr;
}
