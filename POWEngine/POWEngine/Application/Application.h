#pragma once
#include "POWEngine/Core/WorldEntity/WorldEntity.h"
#include "EngineProps.h"

namespace powe
{
    class Core;
    class Window;
    class Application : public std::enable_shared_from_this<Application>
    {
    public:

        Application() = default;
        
        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;
        Application(Application&&) = delete;
        Application& operator=(Application&&) = delete;
        
        virtual ~Application() = default;
        
        void Run();

        static uint8_t WindowIDCounter;

        Window* GetGameWindow() const {return m_GameWindow;}
        
    protected:

        // const SharedPtr<Window>& GetMainWindow() const {return m_MainWindow;}

        // static void GetWindowInfo(uint8_t windowID);
        
        virtual void OnEngineSetUp(EngineProps&) {}
        virtual void OnPreStep(WorldEntity&) {}
        virtual void OnWorldInitialize(WorldEntity&) {}
    
    private:

        OwnedPtr<WorldEntity> m_WorldEntity;
        SharedPtr<Core> m_EngineCore;
        std::unordered_map<uint8_t,OwnedPtr<Window>> m_OpenWindows;
        Window* m_GameWindow;
    };
}

