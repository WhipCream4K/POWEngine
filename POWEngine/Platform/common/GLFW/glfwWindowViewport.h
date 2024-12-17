#pragma once

#include "Renderer/Viewport.h"

namespace powe
{
    class glfwWindow;
    class glfwWindowViewport : public Viewport
    {
    public:

        constexpr glfwWindowViewport(
            glfwWindow& window,
            const glm::uvec2& position,
             const glm::uvec2& size) 
            : Viewport(position, size) 
            , m_WindowHandle(window)
            {
            }

        ~glfwWindowViewport() override = default;
            
        void Bind(RenderContext &context) override;
        void Present(RenderContext &context) override;

    private:

        RefWrap<glfwWindow> m_WindowHandle;
    };
}