#pragma once

#include "Core/Core.h"
#include "Renderer/RenderAPI.h"
#include "AppDesc.h"

namespace powe
{
    class WorldEntity;
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


    protected:

        //virtual void OnAppStartUp(AppDesc& appDesc)
        //{
        //}

        //virtual void OnPreStep(WorldEntity&)
        //{
        //}

        //virtual void OnWorldInitialize(WorldEntity&)
        //{
        //}

    private:

        static uint8_t WindowIDCounter;
        
        //Core m_EngineCore; // core only handlek window, world, render interactions so it can be on stack

        //OwnedPtr<WorldEntity> m_WorldEntity;

        //SharedPtr<Window> m_GameWindow;
        //Renderer m_GameWindowRenderer;

        std::unordered_map<uint8_t, UniquePtr<Window>> m_OpenWindows;
    };
}
