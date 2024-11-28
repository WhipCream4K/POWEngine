#pragma once

namespace powe
{
    class AppEvent
    {
    public:

        AppEvent() = default;
        virtual ~AppEvent() = default;

        virtual void OnSetup() = 0;
        virtual void OnUpdate(float) = 0;
    };
}