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
        virtual bool ShouldUpdate() const noexcept { return true; }
    };
}