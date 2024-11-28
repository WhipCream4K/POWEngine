#pragma once


#include "Core/Window/Window.h"
#include "GLFW/glfw3.h"

namespace powe
{
    class glfwWindow : public powe::Window
    {
    public:

        glfwWindow(std::string_view title, int width, int height);
        ~glfwWindow();

        virtual void SetFullscreen(bool fullscreen, bool borderless = false) override;
        virtual void SetTitle(std::string_view title) override;
        virtual void Resize(uint32_t width, uint32_t height) override;

        virtual bool IsClosed() const noexcept override;
        virtual bool IsFocused() const noexcept override;
        virtual uint32_t GetWidth() const noexcept override;
        virtual uint32_t GetHeight() const noexcept override;
        virtual bool IsFullscreen() const noexcept override;
        virtual std::string_view GetTitle() const noexcept override { return m_Title; }

    private:

        void ResizeCallback(uint32_t width, uint32_t height);

        GLFWwindow* m_WindowHandle;
        std::string m_Title;
        uint32_t m_Width, m_Height;
    };
}


