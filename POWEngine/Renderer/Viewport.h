#pragma once

namespace powe
{
    class Viewport
    {
    public:
        
        enum Flag
        {
            None = 0,
            Color = 1 << 0,
            Depth = 1 << 1,
        };
    };
}

