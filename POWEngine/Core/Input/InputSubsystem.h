#pragma once

namespace powe
{
    class InputSubsystem
    {
    public:

        InputSubsystem() = default;
        virtual ~InputSubsystem() = default;

        virtual void Update(float) = 0;
    };

}