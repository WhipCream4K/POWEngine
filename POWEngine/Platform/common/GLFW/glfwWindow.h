#pragma once


#include "Core/Window/Window.h"

struct GLFWwindow;

namespace powe
{
    class glfwWindowViewport;
    class glfwWindow : public Window
    {
    public:

        glfwWindow(std::string_view title, int width, int height);
        ~glfwWindow();

        virtual void SetTitle(std::string_view title) noexcept override;
        virtual void Resize(uint32_t width, uint32_t height) noexcept override;
        void SetFullscreen(bool fullscreen, bool borderless) noexcept override;
        virtual bool IsClosed() const noexcept override;
        virtual bool IsFocused() const noexcept override;
        virtual uint32_t GetWidth() const noexcept override { return m_Width; }
        virtual uint32_t GetHeight() const noexcept override { return m_Height; }
        virtual bool IsFullscreen() const noexcept override;
        virtual void OnCreate(WindowManager *windowManager) noexcept override;
        virtual std::string_view GetTitle() const noexcept override { return m_Title; }
        virtual SharedPtr<Viewport> GetViewport() const noexcept override { return m_Viewport; }

        GLFWwindow* GetHandle() const noexcept { return m_WindowHandle; }

    private:

        void ResizeCallback(uint32_t width, uint32_t height) noexcept;

        GLFWwindow* m_WindowHandle;
        WindowManager* m_WindowManager;
        std::string m_Title;
        uint32_t m_Width, m_Height;
        SharedPtr<Viewport> m_Viewport;
    };
}


