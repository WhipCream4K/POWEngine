#pragma once

#include "POWEngine/Math/Math.h"

namespace powe
{
    struct WindowProps
    {
        uint32_t width{};
        uint32_t height{};
        std::string windowName{};
        glm::uvec4 clearColor{};

        bool startWithVSync{};
    };

    class Renderer;
    struct EngineProps
    {
        WindowProps winProps{};
        Renderer* renderer{}; 
    };
    
}

