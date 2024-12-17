#pragma once

#include "Core/IModule.h"
#include "Game/Input/InputManager.h"

namespace powe
{
    class Scene;
    class GameEvent;
    class Window;
    class InputManager;
    class Game : public IModule
    {
    public:
        
        using SceneItem = std::pair<std::string, UniquePtr<Scene>>;

        Game();
        ~Game();

        virtual void OnCreate(ModulesManager* modulesManager) override;
        virtual void OnExit(ModulesManager* modulesManager) override;

        Scene* CreateScene(std::string_view sceneName) noexcept;
        void RemoveScene(std::string_view sceneName) noexcept;

        void SetActiveScene(Scene* scene) noexcept;
        Scene* GetActiveScene() const noexcept;

        std::string_view GetSceneName(const Scene* scene) const noexcept;

        void SetBindWindow(Window* window) noexcept;
        Window* GetBindWindow() const noexcept { return m_BindWindow; }
        SharedPtr<Viewport> GetSceneViewport() const noexcept { return m_SceneViewport; }

        InputManager& GetInputManager() noexcept { return m_InputManager; }

    private:

        UnOrderedMap<std::string, UniquePtr<Scene>> m_SceneMap;
        SharedPtr<GameEvent> m_GameEvent;

        InputManager m_InputManager;

        // TODO: Implement viewport of different scene
        SharedPtr<Viewport> m_SceneViewport;

        // The window that the scene is bound to
        Window* m_BindWindow;

    };
}