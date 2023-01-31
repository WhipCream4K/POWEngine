#pragma once
#include "EngineProps.h"
#include "IApplication.h"
#include "POWEngine/Core/Core.h"
#include "POWEngine/Renderer/Renderer.h"

namespace powe
{
    class WorldEntity;
    // class Core;
    class Window;

    class Application : public IApplication, public std::enable_shared_from_this<Application>
    {
    public:
        Application();
        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;
        Application(Application&&) = delete;
        Application& operator=(Application&&) = delete;

        virtual ~Application();

        void Run();

        static uint8_t WindowIDCounter;

        Window* GetGameWindow() const { return m_GameWindow.get(); }

    protected:
        virtual void OnEngineSetUp(EngineProps&)
        {
        }

        virtual void OnPreStep(WorldEntity&)
        {
        }

        virtual void OnWorldInitialize(WorldEntity&)
        {
        }

    private:
        bool TranslateWindowInputs(
            const Window& window,
            WorldEntity& worldEntt
        ) const;


        Core m_EngineCore; // core only handle window, world, render interactions so it can be on stack

        OwnedPtr<WorldEntity> m_WorldEntity;

        OwnedPtr<WorldClock> m_WorldClock;

        OwnedPtr<InputManager> m_InputManager;

        SharedPtr<Window> m_GameWindow;
        Renderer m_GameWindowRenderer;

        // std::unordered_map<uint8_t, OwnedPtr<Window>> m_OpenWindows;
        // Window* m_GameWindow{};
    };
}
