#pragma once
#include "POWEngine/Core/WorldEntity/WorldEntity.h"
#include "EngineProps.h"

namespace powe
{
    class Core;
    class Window;
    class Application
    {
    public:

        Application() = default;
        
        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;
        Application(Application&&) = delete;
        Application& operator=(Application&&) = delete;
        
        virtual ~Application() = default;
        
        void Run();
        
    protected:

        const SharedPtr<Window>& GetMainWindow() const {return m_MainWindow;}

        virtual void OnEngineSetUp(EngineProps&) {}
        virtual void OnWorldInitialize() {}
    
    private:

        SharedPtr<WorldEntity> m_WorldEntity;
        OwnedPtr<Core> m_EngineCore;
        SharedPtr<Window> m_MainWindow;
    };
}

