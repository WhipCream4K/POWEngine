#pragma once

#include "Core/IModule.h"

namespace powe
{
    class Scene;
    class GameEvent;
    class Game : public IModule
    {
    public:

        Game();
        ~Game();

        virtual void OnCreate(ModulesManager* modulesManager) override;
        virtual void OnExit(ModulesManager* modulesManager) override;

        Scene* CreateScene(std::string_view sceneName) noexcept;
        void RemoveScene(std::string_view sceneName) noexcept;

        Scene* GetActiveScene() const noexcept { return m_Scenes.back().get(); }

    private:

        Vector<UniquePtr<Scene>> m_Scenes;
        SharedPtr<GameEvent> m_GameEvent;
    };
}