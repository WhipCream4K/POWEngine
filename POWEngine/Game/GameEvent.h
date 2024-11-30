#pragma once

#include "Core/Application/AppEvent.h"

namespace powe
{
    class Game;
    class GameEvent : public AppEvent
    {
    public:

        GameEvent(Game& game);
        virtual ~GameEvent() = default;

        virtual void OnSetup() override {}
        virtual void OnUpdate(float) override;

        // TODO: Finish this class

    private:

        RefWrap<Game> m_Game;

    };
}