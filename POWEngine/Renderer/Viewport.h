#pragma once

#include <glm/glm.hpp>

namespace powe
{
    class RenderTarget;
    class RenderContext;
    class Viewport
    {
    public:

        constexpr Viewport(const glm::vec2& position, const glm::vec2& size) 
            : m_Position(position), m_Size(size) 
            {
            }

        
        virtual ~Viewport() = default;

        // The method to bind the viewport to be ready for "Pixel transfers" from the render context
        // useful for multiple windows rendering
        virtual void Bind([[maybe_unused]] RenderContext& context) {}

        // The method to present the viewport after "Pixel transfer", useful for multiple windows rendering
        virtual void Present([[maybe_unused]] RenderContext& context) {}

        const glm::uvec2& GetPosition() const noexcept { return m_Position; }
        const glm::uvec2& GetSize() const noexcept { return m_Size; }

        void SetPosition(const glm::uvec2& position) { m_Position = position; }
        void SetSize(const glm::uvec2& size) { m_Size = size; }

    private:

        glm::uvec2 m_Position;
        glm::uvec2 m_Size;
    };
}

