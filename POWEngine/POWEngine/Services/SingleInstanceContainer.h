#pragma once

namespace powe
{
    class Core;
    class Application;
    class SingleInstanceContainer
    {
    public:

        SharedPtr<Core> GetCore() const;
        SharedPtr<Application> GetSharedApplication() const;
        
    private:

        WeakPtr<Core> m_Core;
        WeakPtr<Application> m_Application;
    };
}

