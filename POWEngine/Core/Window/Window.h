#pragma once

#include <string>
#include "Core/CustomTypes.h"

namespace powe
{
    class Window
    {
    public:
        Window(PMRResource* memResource, std::string_view title, int width, int height);
        ~Window();

        void Initialize();


        /**
         * Poll events will fill the event queue with events that have occurred since the last time it was called.
         * It's using the stack allocated event queue of the window.
         */
        void PollEvents();


        void SwapBuffers();
        void SetFullscreen(bool fullscreen);
        void SetTitle(const std::string& title);
        void Resize(int width, int height);

        template <typename T>
        T* GetWindowHandle() const
        {
            return static_cast<T*>(m_WindowHandle.get());
        }


        int GetWidth() const { return m_Width; }
        int GetHeight() const { return m_Height; }
        bool IsFullscreen() const { return m_Fullscreen; }

        static void InitializeLibrary();
        static void TerminateLibrary();

    private:

        std::string m_Title;
        std::pmr::memory_resource* m_MemResource;

        int m_Width;
        int m_Height;
        bool m_Fullscreen;
        
        SharedPtr<void> m_WindowHandle;
    };
}
