#pragma once


namespace powe
{
    // Interface for Window Subsystem
    class Window
    {
    public:    
        
        Window() = default;
        Window(const Window&) = default;
        Window& operator=(const Window&) = default;
        Window(Window&&) = default;
        Window& operator=(Window&&) = default;
        virtual ~Window() = default;

        /**
         * Poll events will fill the event queue with events that have occurred since the last time it was called.
         * It's using the stack allocated event queue of the window.
         */
        // bool PollEvents(EventQueue& eventQueue);
        
        static UniquePtr<Window> Create(const SharedPtr<PMRResource>& allocator,std::string_view windowName, uint32_t width, uint32_t height);

        virtual void SetFullscreen(bool fullscreen, bool borderless) = 0;
        virtual void SetTitle(std::string_view title)= 0;
        virtual void Resize(uint32_t width, uint32_t height) = 0;
        
        virtual bool IsClosed() const noexcept = 0;
        virtual bool IsFocused() const noexcept = 0;
        virtual uint32_t GetWidth() const noexcept = 0;
        virtual uint32_t GetHeight() const noexcept = 0;
        virtual bool IsFullscreen() const noexcept = 0;
        virtual std::string_view GetTitle() const noexcept = 0;

    };
}
