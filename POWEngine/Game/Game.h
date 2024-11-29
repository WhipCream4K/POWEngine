#pragma once

#include "Core/IModule.h"

namespace powe
{
    class Scene;
    class GameEvent;
    class Window;
    class Game : public IModule
    {
    public:

        Game();
        ~Game();

        virtual void OnCreate(ModulesManager* modulesManager) override;
        virtual void OnExit(ModulesManager* modulesManager) override;

        SharedPtr<Scene> CreateScene(std::string_view sceneName) noexcept;
        void RemoveScene(std::string_view sceneName) noexcept;

        Scene* GetActiveScene() const noexcept { return m_Scenes.back().get(); }
        SharedPtr<Window> GetBindWindow() const noexcept { return m_BindWindow.lock(); }
        SharedPtr<PMRResource> GetResource() const noexcept;

    private:

        Vector<SharedPtr<Scene>> m_Scenes;
        SharedPtr<GameEvent> m_GameEvent;

        // The window that the scene is bound to
        WeakPtr<Window> m_BindWindow;
    };
}