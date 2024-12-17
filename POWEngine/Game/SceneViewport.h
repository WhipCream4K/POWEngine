#pragma once

#include "Renderer/Viewport.h"

namespace powe
{
    class SceneViewport : public Viewport
    {
    public:

        SceneViewport(const glm::vec2& position, const glm::vec2& size) 
        : Viewport(position, size) {}

        void Bind(RenderContext& context) override;
        void Present(RenderContext& context) override;

        void BindWindow(Window* window) noexcept;

    private:

        Window* m_BindWindow;
        Viewport* m_WindowViewport;
    };
}