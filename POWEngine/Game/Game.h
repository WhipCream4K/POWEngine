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

        Scene* CreateScene(std::string_view sceneName) noexcept;
        void RemoveScene(std::string_view sceneName) noexcept;

        Scene* GetActiveScene() const noexcept { return m_ActiveScene; }

        std::string_view GetSceneName(const Scene* scene) const noexcept;

        void SetBindWindow(Window* window) noexcept { m_BindWindow = window; }
        Window* GetBindWindow() const noexcept { return m_BindWindow; }

        SharedPtr<PMRResource> GetResource() const noexcept;

    private:

        UnOrderedMap<std::string, UniquePtr<Scene>> m_SceneMap;
        Scene* m_ActiveScene;
        SharedPtr<GameEvent> m_GameEvent;

        // The window that the scene is bound to
        Window* m_BindWindow;

    };
}