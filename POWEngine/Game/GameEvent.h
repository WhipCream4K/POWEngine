#pragma once

#include "Core/Application/AppEvent.h"

namespace powe
{
    class Game;
    class Scene;
    class GameEvent : public AppEvent
    {
    public:

        GameEvent(Game& game);
        virtual ~GameEvent() = default;

        Scene* GetActiveScene() const noexcept { return m_ActiveScene; }
        void SetActiveScene(Scene* scene) noexcept { m_ActiveScene = scene; }

        virtual void OnSetup() override {}
        virtual void OnUpdate(float) override;

    private:

        RefWrap<Game> m_Game;
        Scene* m_ActiveScene;
    };
}