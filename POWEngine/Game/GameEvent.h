#pragma once

#include "Core/Application/AppEvent.h"

namespace powe
{
    class GameEvent : public AppEvent
    {
    public:

        GameEvent() = default;
        virtual ~GameEvent() = default;

        virtual void OnSetup() override {}
        virtual void OnUpdate(float) override;

        // TODO: Finish this class

    };
}