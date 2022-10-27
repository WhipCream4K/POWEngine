#pragma once



namespace powe
{
    struct WindowProps
    {
        uint32_t width{};
        uint32_t height{};
        std::string windowName{};

        bool startWithVSync{};
    };

    class Renderer;
    struct EngineProps
    {
        WindowProps winProps{};
        SharedPtr<Renderer> renderer{}; 
    };
    
}

